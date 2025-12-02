import rclpy
from rclpy.node import Node
import threading
from std_msgs.msg import Int16

class DesktopNode(Node):

    def __init__(self):
        super().__init__('desktop_node')
        self.publisher_ = self.create_publisher(Int16, 'input', 10)
        self.get_logger().info("Created input publisher")

        self.input_thread = threading.Thread(target=self.publish_input)
        self.input_thread.daemon = True
        self.input_thread.start()
        self.get_logger().info("Input thread started.")

    def publish_input(self):
        while rclpy.ok():
            try:
                user_input = input('Input number (0-10)')
                if (not user_input.strip()) and user_input.strip().isdigit():
                    self.get_logger().warn("Wrong input format")
                    continue

                user_num = int(user_input)

                if 0 <= user_num <= 10:
                    msg = Int16()
                    msg.data = user_num
                    self.publisher_.publish(msg)
                    self.get_logger().info(f"Published: {msg.data}")
            except Exception as e:
                self.get_logger().warn(f"Ran into input exception: {e}")
                break


def main(args=None):
    rclpy.init(args=args)
    desktop_node = DesktopNode()
    try:
        rclpy.spin(desktop_node)
    except KeyboardInterrupt:
        pass
    finally:
        desktop_node.destroy_node()
        rclpy.shutdown()

if __name__ == "__main__":
    main()