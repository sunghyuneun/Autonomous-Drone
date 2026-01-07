import serial
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Odometry
from sensor_msgs.msg import Imu
from geometry_msgs.msg import Quaternion, TransformStamped
import math
import tf_transformations

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 115200
WHEEL_SEPARATION = .206 #206 mm

class ArduinoPublisher(Node):
    def __init__(self):
        super().__init__('arduino_publisher')

        self.imu_publisher = self.create_publisher(Imu, 'imu/raw_data', 10)
        self.get_logger().info("Created IMU Data Publisher")

        self.odom_publisher = self.create_publisher(Odometry, 'odom', 10)
        self.get_logger().info("Created Odometry Publisher")

        try:
            self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout = .1)
            self.get_logger().info("Connected to Arduino Serial")
        except Exception as e:
            self.get_logger().error(f"Could not open serial port: {e}")

        self.x = 0.0
        self.y = 0.0
        self.th = 0.0
        self.last_time = self.get_clock().now()

        self.create_timer(0.02,self.publish_info)


    def publish_info(self):
        #write_data = "L0.1R0.1" + '\n'
        #self.ser.write(write_data.encode("utf-8"))
        if hasattr(self,'ser') and self.ser.in_waiting > 0:
            try:
                line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                
                if line.startswith("V:"):
                    try:
                        data_parts = [float(val) for val in line[2:].split(',')]
                        
                        if len(data_parts) != 8:
                            self.get_logger().warn(f"Data packet length mismatch, length = {data_parts}")
                            return

                        
                        current_time = self.get_clock().now()

                        v_right, v_left = data_parts[0],data_parts[1]

                        #IMU stuff. 
                        imu_msg = Imu()
                        imu_msg.header.stamp = current_time.to_msg()
                        imu_msg.header.frame_id = "imu"

                        imu_msg.linear_acceleration.x = data_parts[2]
                        imu_msg.linear_acceleration.y = data_parts[3]
                        imu_msg.linear_acceleration.z = data_parts[4]
                        
                        imu_msg.angular_velocity.x = data_parts[5]
                        imu_msg.angular_velocity.x = data_parts[6]
                        imu_msg.angular_velocity.x = data_parts[7]

                        self.imu_publisher.publish(imu_msg)

                        #Odom stuff
                        dt = (current_time - self.last_time).nanoseconds / 1e9

                        v_linear = (v_right + v_left) / 2.0
                        v_angular = (v_right - v_left) / WHEEL_SEPARATION

                        self.x += (v_linear * math.cos(self.th)) * dt
                        self.y += (v_linear * math.sin(self.th)) * dt
                        self.th += v_angular * dt

                        q = tf_transformations.quaternion_from_euler(0, 0, self.th)
                        odom_msg = Odometry()
                        odom_msg.header.stamp = current_time.to_msg()
                        odom_msg.header.frame_id = "odom"
                        odom_msg.child_frame_id = 'box_body'

                        odom_msg.pose.pose.position.x = self.x
                        odom_msg.pose.pose.position.y = self.y
                        odom_msg.pose.pose.orientation = Quaternion(x=q[0], y=q[1], z=q[2], w=q[3])

                        odom_msg.twist.twist.linear.x = v_linear
                        odom_msg.twist.twist.angular.z = v_angular

                        # 5. Publish
                        self.odom_publisher.publish(odom_msg)
                        self.last_time = current_time

                    except ValueError:
                        self.get_logger().error(f"Failed to parse data: {line[2:]}")

            except Exception as e:
                self.get_logger().error(f"Error reading serial data: {e}")

def main(args = None):
    rclpy.init(args=args)

    arduino_publisher = ArduinoPublisher()

    try:
        rclpy.spin(arduino_publisher)
    except KeyboardInterrupt:
        pass
    finally:
        arduino_publisher.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()