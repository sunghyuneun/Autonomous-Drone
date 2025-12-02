import serial
import rclpy
from rclpy.node import Node
from std_msgs.msg import Int16

SERIAL_PORT = '/dev/ttyACM0'
BAUD_RATE = 9600
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout = 1)

class RPiSubscriber(Node):
    def __init__(self):
        super().__init__('rpi_subscriber')
        self.subscription = self.create_subscription(Int16,'input',self.arduino_write, 10)
        self.subscription

    def arduino_write(self, msg):
        write_data = str(msg.data) + '\n'
        ser.write(write_data.encode("utf-8"))
        self.get_logger().info(f"Sent: {msg}")

def main(args = None):
    rclpy.init(args=args)

    rpi_subscriber = RPiSubscriber()
    rclpy.spin(rpi_subscriber)

    rpi_subscriber.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()