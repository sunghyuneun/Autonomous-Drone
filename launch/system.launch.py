#!/usr/bin/env python3

import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.actions import LogInfo
from launch.actions import ExecuteProcess
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node

def generate_launch_description():
    #URDF File
    urdf_file = "/home/hyun/Autonomous-Drone/CAD/robot.urdf"
    with open(urdf_file, 'r') as infp:
        robot_desc = infp.read()

    rviz_config_dir = os.path.join(
        get_package_share_directory('rplidar_ros'),
        'rviz',
        'rplidar_ros.rviz')
        

    channel_type =  LaunchConfiguration('channel_type', default='serial')
    serial_port = LaunchConfiguration('serial_port', default='/dev/ttyUSB0')
    serial_baudrate = LaunchConfiguration('serial_baudrate', default='460800')
    frame_id = LaunchConfiguration('frame_id', default='lidar')
    inverted = LaunchConfiguration('inverted', default='false')
    angle_compensate = LaunchConfiguration('angle_compensate', default='true')
    scan_mode = LaunchConfiguration('scan_mode', default='Standard')

    #foxglove_studio = ExecuteProcess(cmd=["foxglove-studio"])

    foxglove_bridge = ExecuteProcess(cmd=["ros2", "launch", "foxglove_bridge", "foxglove_bridge_launch.xml"])

    arduino_listener_file = "/home/hyun/Autonomous-Drone/RaspPiCode/arduino_listener.py"

    arduino_listener = ExecuteProcess(cmd=["python3", arduino_listener_file], output = 'screen')

    robot_localization_file_path = "/home/hyun/Autonomous-Drone/config/ekf.yaml"

    slam_file_path = "/home/hyun/Autonomous-Drone/config/mapper_params_online_async.yaml"

    return LaunchDescription([
        #Argument Declaration
        DeclareLaunchArgument('channel_type', default_value=channel_type),
        DeclareLaunchArgument('serial_port', default_value=serial_port),
        DeclareLaunchArgument('serial_baudrate', default_value=serial_baudrate),
        DeclareLaunchArgument('frame_id', default_value=frame_id),
        DeclareLaunchArgument('inverted', default_value=inverted),
        DeclareLaunchArgument('angle_compensate', default_value=angle_compensate),
        DeclareLaunchArgument('scan_mode', default_value=scan_mode),

        #Nodes
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            name='robot_state_publisher',
            parameters=[{'robot_description': robot_desc}],
            arguments=[urdf_file]),

        Node(
            package='rplidar_ros',
            executable='rplidar_node',
            name='rplidar_node',
            parameters=[{'channel_type':channel_type,
                         'serial_port': serial_port,
                         'serial_baudrate': serial_baudrate,
                         'frame_id': frame_id,
                         'inverted': inverted,
                         'angle_compensate': angle_compensate,
                           'scan_mode': scan_mode
                         }],
            output='screen'),

        #foxglove_studio,
        foxglove_bridge,
        arduino_listener,

        Node(
            package='robot_localization',
            executable='ekf_node',  
            name='ekf_filter_node',
            output='screen',
            parameters=[robot_localization_file_path]),
        
        Node(
            package='slam_toolbox',
            executable='online_async_launch',  
            name='slam_node',
            output='screen',
            parameters=[{'slam_params_file':slam_file_path}]),
        #Node(
        #    package='rviz2',
        #    executable='rviz2',
        #    name='rviz2',
        #    arguments=['-d', rviz_config_dir],
        #    output='screen'),
    ])