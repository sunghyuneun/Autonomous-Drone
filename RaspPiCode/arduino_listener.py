import serial
import rclpy
from rclpy.node import Node
from std_msgs.msg import Int16
from sensor_msgs.msg import Imu

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 115200
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout = 1)

class ArduinoPublisher(Node):
    def __init__(self):
        super().__init__('arduino_publisher')

        self.imu_publisher = self.create_publisher(Int16, 'IMU', 10)
        self.wheel_publisher = self.create_publisher()

    def arduino_write(self, msg):
        write_data = str(msg.data) + '\n'
        ser.write(write_data.encode("utf-8"))
        self.get_logger().info(f"Sent: {msg.data}")

def main(args = None):
    rclpy.init(args=args)

    rpi_subscriber = ArduinoPublisher()
    rclpy.spin(rpi_subscriber)

    rpi_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()