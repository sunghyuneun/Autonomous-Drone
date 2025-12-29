from launch import LaunchDescription
from launch_ros.actions import Node
import os
from ament_index_python.packages import get_package_share_directory

def generate_launch_description():
    urdf_file = "../CAD/robot.urdf"
    with open(urdf_file, 'r') as infp:
        robot_desc = infp.read()
        
    return LaunchDescription([
        Node(
            package='idk what this package is',
            executable='arduino_listener.py',
            name="arduino_listener",
            output="screen"
        ),
        Node(
            package='idk what this package is either',
            executable='the executable',
            name="a name",
            output="yeah lol"
        )
    ])