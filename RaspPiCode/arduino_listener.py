import serial
import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray
from sensor_msgs.msg import Imu

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 115200

class ArduinoPublisher(Node):
    def __init__(self):
        super().__init__('arduino_publisher')

        self.imu_publisher = self.create_publisher(Imu, 'imu/raw_data', 10)
        self.get_logger().info("Created IMU Data Publisher")

        self.wheel_publisher = self.create_publisher(Float32MultiArray, 'Wheel_Vel', 10)
        self.get_logger().info("Created Wheel Vel Publisher")

        try:
            self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout = .1)
            self.get_logger().info("Connected to Arduino Serial")
        except Exception as e:
            self.get_logger().error(f"Could not open serial port: {e}")

        self.create_timer(0.02,self.publish_info)


    def publish_info(self):
        if hasattr(self,'ser') and self.ser.in_waiting > 0:
            try:
                line = self.ser.readline().decode('utf-8', errors='ignore').strip()
                
                if line.startswith("V:"):
                    try:
                        data_parts = [float(val) for val in line[2:].split(',')]
                        
                        if len(data_parts) != 8:
                            self.get_logger.warn(f"Data packet length mismatch")
                            return
                        
                        wheel_vel_msg = Float32MultiArray()
                        wheel_vel_msg.data = [data_parts[0],data_parts[1]]
                        self.wheel_publisher.publish(wheel_vel_msg)

                        imu_msg = Imu()
                        imu_msg.header.stamp = self.get_clock().now().to_msg()
                        imu_msg.header.frame_id = "imu_link"

                        imu_msg.linear_acceleration.x = data_parts[2]
                        imu_msg.linear_acceleration.y = data_parts[3]
                        imu_msg.linear_acceleration.z = data_parts[4]
                        
                        imu_msg.angular_velocity.x = data_parts[5]
                        imu_msg.angular_velocity.x = data_parts[6]
                        imu_msg.angular_velocity.x = data_parts[7]

                        self.imu_publisher.publish(imu_msg)

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