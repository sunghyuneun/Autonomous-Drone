import rclpy
from rclpy.node import Node

from std_msgs.msg import Int16

class DesktopNode(Node):

    def __init__(self):
        super().__init__('desktop_node')
        self.publisher_ = self.create_publisher(Int16, 'input', 10)

    def publish_msg(self):
        msg = Int16()
        msg.data = int(input('Input number (0-10)'))
        self.publisher_.publish(msg)


def main(args=None):
    rclpy.init(args=args)
    desktop_node = DesktopNode()
    rclpy.spin(desktop_node)

    desktop_node.destroy_node()
    rclpy.shutdown()

if __name__ == "__main__":
    main()