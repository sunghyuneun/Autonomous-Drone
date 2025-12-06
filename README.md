# Overview
Similar to Turtlebot 3

- Features
    - Raspberry Pi 5
    - Arduino
    - DC Motors
    - Odometry
        - Encoders
        - IMU
        - LiDAR
    - SLAM (w/ LiDAR)
    - Obstacle Avoidance with PPO


# Progress

## Initial Pipeline
Created a ROS2 pipeline from Desktop - Raspberry Pi - Arduino
Desktop and Raspberry Pi share same network and domain, so commands can be sent from the Desktop, received at Raspberry Pi, write instructions to Arduino, which does PWM to an LED. 

## LiDAR
Visualized LiDAR information from Raspberry Pi onto Desktop using RViz2.