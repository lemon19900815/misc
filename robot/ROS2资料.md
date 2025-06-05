# ROS2资料学习整理

参考资料：

- [官方资料](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html)
- [动手学ROS2](https://fishros.com/d2lros2)
- [ros-wiki](https://wiki.ros.org/cn)
- [ros-stackoverflow](https://robotics.stackexchange.com/)
- [github-fishbot](https://github.com/fishros/fishbot)
- [awesome-ros2](https://github.com/fkromer/awesome-ros2)
- [ros-visualization_tutorials](https://github.com/ros-visualization/visualization_tutorials)
- [ros社区论坛](https://www.ros.org/blog/community/)
- [ros-index包搜索](https://index.ros.org/)
- [ros-github](https://github.com/ros2/ros2)
-  [ros1-bridge](https://github.com/ros2/ros1_bridge)



## 1. ROS2安装

| 系统         | 系统代号 | ROS2 版本                                                    |
| ------------ | -------- | ------------------------------------------------------------ |
| Ubuntu 20.04 | focal    | [foxy](https://docs.ros.org/en/foxy/Installation/Ubuntu-Install-Debians.html) |
| Ubuntu 22.04 | jammy    | [humble](https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debs.html) |
| Ubuntu 24.04 | noble    | [jazzy](https://docs.ros.org/en/jazzy/Installation/Ubuntu-Install-Debs.html) |

选择与系统匹配的版本进行安装。



[一键安装ROS2](https://fishros.com/d2lros2/#/humble/chapt1/get_started/3.动手安装ROS2?id=_1一键安装ros2)

```sh
wget http://fishros.com/install -O fishros && . fishros
```



## 2. ROS2使用

`rosdep` 是 ROS (Robot Operating System) 中的一个命令行工具，主要用于简化在不同操作系统上安装 ROS 依赖包的过程。它帮助你自动化安装 ROS 项目所需的系统级别依赖，从而使得 ROS 项目可以在不同平台（如 Ubuntu、Debian 等）上更轻松地构建和运行。

- 初始化 rosdep：

  ```sh
  sudo rosdep init
  rosdep update
  ```

- 安装依赖

  ```sh
  rosdep install <package_name>
  ```

- 在工作空间安装依赖

  ```sh
  rosdep install --from-paths src --ignore-src -r -y
  ```

  这条命令会从当前工作空间的 `src` 目录查找所有包，并自动安装它们所需的所有依赖。

- 检查依赖是否安装

  ```sh
  # 这个命令检查指定包是否有未安装的依赖
  rosdep check <package_name>
  ```

`rosdep` 是 ROS 开发中非常重要的工具，它简化了依赖管理和跨平台的配置。通过自动安装所需的依赖，它大大减少了开发者的工作量，使得 ROS 项目的构建更加顺利和高效。



### 2.1 启动ROS2节点

- 直接运行节点

  ```sh
  ros2 run cpp_pubsub talker
  ros2 run cpp_pubsub listener
  ```

  

- [使用launch运行节点](https://fishros.com/d2lros2/#/humble/chapt5/get_started/1.启动管理工具-Launch?id=_1启动管理工具-launch)

  ```python
import launch
  from launch import LaunchDescription
  from launch.actions import DeclareLaunchArgument, LogInfo
  from launch_ros.actions import Node
  
  def generate_launch_description():
      return LaunchDescription([
          DeclareLaunchArgument('config_file', default_value='config.yaml', description='Path to the config file'),
          Node(
              package='cpp_pubsub',
              executable='mytalker',
              name='minimal_publisher',
              parameters=[{'config_file': 'config.yaml'}],
              output='screen'
          ),
          # 多个节点就新增Node即可...
      ])
  ```
  
  运行：`ros2 launch cpp_pubsub talker.launch.py`
  
  - for c++：`ament_cmake`类型功能包，修改CMakeLists.txt
  
    ```cmake
    install(DIRECTORY launch
      DESTINATION share/${PROJECT_NAME})
    ```
  
    
  
  - for python：`ament_python`功能包版，修改setup.py
  
    ```py
    from setuptools import setup
    from glob import glob
    import os
    
    setup(
        name=package_name,
        version='0.0.0',
        packages=[package_name],
        data_files=[
            ('share/ament_index/resource_index/packages',
                ['resource/' + package_name]),
            ('share/' + package_name, ['package.xml']),
            (os.path.join('share', package_name, 'launch'), glob('launch/*.launch.py')), # 新增
        ],
        },
    )	
    ```



### 2.2 ROS节点参数查询

```sh
# 查询当前开启的ros节点可以设置的参数
ros2 param list

# 如果看不懂，还可以有一个方法详细查看一个参数的信息。
ros2 param describe <node_name> <param_name>

# 启动时设置节点参数
ros2 run realsense2_camera realsense2_camera_node --ros-args -p enable_color:=false -p spatial_filter.enable:=true -p temporal_filter.enable:=true

# 参数的组成由名字和值（键值组成），下面这个命令行工具可以帮助我们获取参数的值
ros2 param get <node_name> <param_name>

# 使用下面的指令来设置参数的值
ros2 param set <node_name> <parameter_name> <value>

# 查询节点参数描述
ros2 param describe <your_node_name> <param_name>
```




### 2.3 [RQT工具](https://fishros.com/d2lros2/#/humble/chapt5/get_started/4.常用调试小工具-RQT?id=_4-rqt工具)

```
# rqt可视化监控工具
rqt

# GUI查看/过滤日志
ros2 run rqt_console rqt_console

# 主题显示
ros2 run rqt_topic rqt_topic

# 节点日志存储于~/.ros/log/目录下
```



### 2.4 [检查ros2的设置](https://docs.ros.org/en/rolling/Tutorials/Beginner-Client-Libraries/Getting-Started-With-Ros2doctor.html)

```sh
# 这将检查所有的安装模块并返回警告和错误。
ros2 doctor

# 如果您的ROS 2设置处于完美状态，您将看到类似于以下内容的消息：
All <n> checks passed

# ros2 doctor还可以检查主题是否有被发布订阅等，您将看到类似于以下内容的消息：
# 可用于诊断ros2节点系统是否工作正常；
UserWarning: Publisher without subscriber detected on /topic.
UserWarning: Subscriber without publisher detected on /topic.

# 通过回显topic的消息也可以辅助诊断故障
ros2 topic list
ros2 topic echo /mytopic

# 查看主题发布的数据类型等信息
$ ros2 topic info topic_name
Type: sensor_msgs/msg/Image
Publisher count: 1
Subscription count: 1

# 查看主题详情，包括发布者是谁，订阅者是谁
$ ros2 topic info /robot_description --verbose

# 查看发布主题结构msg的数据详情
$ ros2 interface show sensor_msgs/msg/Image

# 获取完整报告，
ros2 doctor --report

" 它将返回分为7组的信息列表：
NETWORK CONFIGURATION
...

PACKAGE VERSIONS
...

PLATFORM INFORMATION
...

QOS COMPATIBILITY LIST
...

RMW MIDDLEWARE
...

ROS 2 INFORMATION
...

TOPIC LIST
...
"

# ros2 doctor将通知您ROS 2设置和运行系统中的问题。您可以通过使用--report参数来深入了解这些警告背后的信息。
# 请记住，ros2 doctor不是调试工具；它对代码中或系统实现方面的错误没有帮助。
```



### 2.5 [时光记录仪之rosbag2](https://fishros.com/d2lros2/#/humble/chapt5/get_started/5.数据录播工具-rosbag?id=_5时光记录仪之rosbag2)

```sh
# 记录主题数据：可同时记录多个主题数据
ros2 bag record topic-name1 topic-name2

# 记录所有话题
ros2 bag record -a

# -o 指定记录输出的文件名
ros2 bag record -o file-name topic-name

# 我们在播放一个视频前，可以通过文件信息查看视频的相关信息，比如话题记录的时间，大小，类型，数量
ros2 bag info bag-file

# 接着我们就可以重新的播放数据,使用下面的指令可以播放数据
# @Note：如果提示没有找到主题信息，可能是录制时未找到指定的主题
ros2 bag play xxx.db3

# 使用ros2的topic的指令来查看数据
ros2 topic echo /topic_name

# -r 选项可以修改播放速率，比如 -r 值，比如 -r 10,就是10倍速，十倍速播放话题
ros2 bag play rosbag2_2021_10_03-15_31_41_0.db3 -r 10

#  -l 循环播放
ros2 bag play rosbag2_2021_10_03-15_31_41_0.db3  -l

# 播放单个话题
ros2 bag play rosbag2_2021_10_03-15_31_41_0.db3 --topics /topic_name
```



### 2.6 仿真工具-Gazebo

[gazebo入门-古月居](https://zhuanlan.zhihu.com/p/54392010)

[gazebo-fishros](https://fishros.com/d2lros2/#/humble/chapt5/get_started/6.兼容仿真工具-Gazebo?id=_6兼容仿真工具-gazebo)

- RVIZ2是用来可视化数据的软件，核心要义是将数据展示出来（我们不生产数据只做数据的搬运工）
- Gazebo是用于模拟真实环境生产数据的（我们不搬运数据只做数据的生产者)
- Gazebo是独立于ROS/ROS2的软件（还有很多仿真软件可以用ROS/ROS2）
- ROS2和Gazebo之间的桥梁是：gazebo_ros_pkgs



[为机器人URDF模型注入物理属性](https://fishros.com/d2lros2/#/humble/chapt9/get_started/2.给两轮差速机器人添加物理参数?id=_2为机器人urdf模型注入物理属性)

机器人仿真就是用软件来模拟硬件的特性，那么我们必须要告诉仿真平台机器人各个关节的物理属性，比如：

- 有多重，
- 有多大的惯性
- 重心在哪
- 碰撞边界在哪
- 关节的上下界限
- 其他的一些必要信息等等

```xml
<?xml version="1.0"?>
<robot name="fishbot">
  <!-- Robot Footprint -->
  <link name="base_footprint"/>

  <joint name="base_joint" type="fixed">
    <parent link="base_footprint"/>
    <child link="base_link"/>
    <origin xyz="0.0 0.0 0.076" rpy="0 0 0"/>
  </joint>

  <!-- base link -->
  <link name="base_link">
    <visual>
      <origin xyz="0 0 0.0" rpy="0 0 0"/>
      <geometry>
        <cylinder length="0.12" radius="0.10"/>
      </geometry>
      <material name="blue">
        <color rgba="0.1 0.1 1.0 0.5" />
      </material>
    </visual>
    <collision>
      <origin xyz="0 0 0.0" rpy="0 0 0"/>
      <geometry>
        <cylinder length="0.12" radius="0.10"/>
      </geometry>
      <material name="blue">
        <color rgba="0.1 0.1 1.0 0.5" />
      </material>
    </collision>
    <inertial>
      <mass value="0.2"/>
      <inertia ixx="0.0122666" ixy="0" ixz="0" iyy="0.0122666" iyz="0" izz="0.02"/>
    </inertial>
  </link>

  <!-- laser link -->
  <link name="laser_link">
    <visual>
      <origin xyz="0 0 0" rpy="0 0 0"/>
      <geometry>
        <cylinder length="0.02" radius="0.02"/>
      </geometry>
      <material name="black">
        <color rgba="0.0 0.0 0.0 0.5" />
      </material>
    </visual>
    <collision>
      <origin xyz="0 0 0" rpy="0 0 0"/>
      <geometry>
        <cylinder length="0.02" radius="0.02"/>
      </geometry>
      <material name="black">
        <color rgba="0.0 0.0 0.0 0.5" />
      </material>
    </collision>
    <inertial>
      <mass value="0.1"/>
      <inertia ixx="0.000190416666667" ixy="0" ixz="0" iyy="0.0001904" iyz="0" izz="0.00036"/>
    </inertial>
  </link>

  <!-- laser joint -->
  <joint name="laser_joint" type="fixed">
    <parent link="base_link" />
    <child link="laser_link" />
    <origin xyz="0 0 0.075" />
  </joint>

  <link name="imu_link">
    <visual>
      <origin xyz="0 0 0.0" rpy="0 0 0"/>
      <geometry>
        <box size="0.02 0.02 0.02"/>
      </geometry>
    </visual>
    <collision>
      <origin xyz="0 0 0.0" rpy="0 0 0"/>
      <geometry>
        <box size="0.02 0.02 0.02"/>
      </geometry>
    </collision>
    <inertial>
      <mass value="0.1"/>
      <inertia ixx="0.000190416666667" ixy="0" ixz="0" iyy="0.0001904" iyz="0" izz="0.00036"/>
    </inertial>
  </link>

  <!-- imu joint -->
  <joint name="imu_joint" type="fixed">
    <parent link="base_link" />
    <child link="imu_link" />
    <origin xyz="0 0 0.02" />
  </joint>

  <link name="left_wheel_link">
    <visual>
      <origin xyz="0 0 0" rpy="1.57079 0 0"/>
      <geometry>
        <cylinder length="0.04" radius="0.032"/>
      </geometry>
      <material name="black">
        <color rgba="0.0 0.0 0.0 0.5" />
      </material>
    </visual>
    <collision>
      <origin xyz="0 0 0" rpy="1.57079 0 0"/>
      <geometry>
        <cylinder length="0.04" radius="0.032"/>
      </geometry>
      <material name="black">
        <color rgba="0.0 0.0 0.0 0.5" />
      </material>
    </collision>
    <inertial>
      <mass value="0.2"/>
      <inertia ixx="0.000190416666667" ixy="0" ixz="0" iyy="0.0001904" iyz="0" izz="0.00036"/>
    </inertial>
  </link>

  <link name="right_wheel_link">
    <visual>
      <origin xyz="0 0 0" rpy="1.57079 0 0"/>
      <geometry>
        <cylinder length="0.04" radius="0.032"/>
      </geometry>
      <material name="black">
        <color rgba="0.0 0.0 0.0 0.5" />
      </material>
    </visual>
    <collision>
      <origin xyz="0 0 0" rpy="1.57079 0 0"/>
      <geometry>
        <cylinder length="0.04" radius="0.032"/>
      </geometry>
      <material name="black">
        <color rgba="0.0 0.0 0.0 0.5" />
      </material>
    </collision>
    <inertial>
      <mass value="0.2"/>
      <inertia ixx="0.000190416666667" ixy="0" ixz="0" iyy="0.0001904" iyz="0" izz="0.00036"/>
    </inertial>
  </link>

  <joint name="left_wheel_joint" type="continuous">
    <parent link="base_link" />
    <child link="left_wheel_link" />
    <origin xyz="-0.02 0.10 -0.06" />
    <axis xyz="0 1 0" />
  </joint>

  <joint name="right_wheel_joint" type="continuous">
    <parent link="base_link" />
    <child link="right_wheel_link" />
    <origin xyz="-0.02 -0.10 -0.06" />
    <axis xyz="0 1 0" />
  </joint>

  <link name="caster_link">
    <visual>
      <origin xyz="0 0 0" rpy="1.57079 0 0"/>
      <geometry>
        <sphere radius="0.016"/>
      </geometry>
      <material name="black">
        <color rgba="0.0 0.0 0.0 0.5" />
      </material>
    </visual>
    <collision>
      <origin xyz="0 0 0" rpy="1.57079 0 0"/>
      <geometry>
        <sphere radius="0.016"/>
      </geometry>
      <material name="black">
        <color rgba="0.0 0.0 0.0 0.5" />
      </material>
    </collision>
    <inertial>
      <mass value="0.02"/>
      <inertia ixx="0.000190416666667" ixy="0" ixz="0" iyy="0.0001904" iyz="0" izz="0.00036"/>
    </inertial>
  </link>

  <joint name="caster_joint" type="fixed">
    <parent link="base_link" />
    <child link="caster_link" />
    <origin xyz="0.06 0.0 -0.076" />
    <axis xyz="0 1 0" />
  </joint>

  <gazebo reference="caster_link">
    <material>Gazebo/Black</material>
  </gazebo>

  <gazebo reference="caster_link">
    <mu1 value="0.0"/>
    <mu2 value="0.0"/>
    <kp value="1000000.0" />
    <kd value="10.0" />
    <!-- <fdir1 value="0 0 1"/> -->
  </gazebo>

  <gazebo>
    <plugin name='diff_drive' filename='libgazebo_ros_diff_drive.so'>
      <ros>
        <namespace>/</namespace>
        <remapping>cmd_vel:=cmd_vel</remapping>
        <remapping>odom:=odom</remapping>
      </ros>
      <update_rate>30</update_rate>
      <!-- wheels -->
      <!-- <left_joint>left_wheel_joint</left_joint> -->
      <!-- <right_joint>right_wheel_joint</right_joint> -->
      <left_joint>left_wheel_joint</left_joint>
      <right_joint>right_wheel_joint</right_joint>
      <!-- kinematics -->
      <wheel_separation>0.2</wheel_separation>
      <wheel_diameter>0.065</wheel_diameter>
      <!-- limits -->
      <max_wheel_torque>20</max_wheel_torque>
      <max_wheel_acceleration>1.0</max_wheel_acceleration>
      <!-- output -->
      <publish_odom>true</publish_odom>
      <publish_odom_tf>true</publish_odom_tf>
      <publish_wheel_tf>false</publish_wheel_tf>
      <odometry_frame>odom</odometry_frame>
      <robot_base_frame>base_footprint</robot_base_frame>
    </plugin>
    <plugin name="fishbot_joint_state" filename="libgazebo_ros_joint_state_publisher.so">
      <ros>
        <remapping>~/out:=joint_states</remapping>
      </ros>
      <update_rate>30</update_rate>
      <joint_name>right_wheel_joint</joint_name>
      <joint_name>left_wheel_joint</joint_name>
    </plugin>
  </gazebo>

  <gazebo reference="laser_link">
    <material>Gazebo/Black</material>
  </gazebo>

  <gazebo reference="imu_link">
    <sensor name="imu_sensor" type="imu">
      <plugin filename="libgazebo_ros_imu_sensor.so" name="imu_plugin">
        <ros>
          <namespace>/</namespace>
          <remapping>~/out:=imu</remapping>
        </ros>
        <initial_orientation_as_reference>false</initial_orientation_as_reference>
      </plugin>
      <always_on>true</always_on>
      <update_rate>100</update_rate>
      <visualize>true</visualize>
      <imu>
        <angular_velocity>
          <x>
            <noise type="gaussian">
              <mean>0.0</mean>
              <stddev>2e-4</stddev>
              <bias_mean>0.0000075</bias_mean>
              <bias_stddev>0.0000008</bias_stddev>
            </noise>
          </x>
          <y>
            <noise type="gaussian">
              <mean>0.0</mean>
              <stddev>2e-4</stddev>
              <bias_mean>0.0000075</bias_mean>
              <bias_stddev>0.0000008</bias_stddev>
            </noise>
          </y>
          <z>
            <noise type="gaussian">
              <mean>0.0</mean>
              <stddev>2e-4</stddev>
              <bias_mean>0.0000075</bias_mean>
              <bias_stddev>0.0000008</bias_stddev>
            </noise>
          </z>
        </angular_velocity>
        <linear_acceleration>
          <x>
            <noise type="gaussian">
              <mean>0.0</mean>
              <stddev>1.7e-2</stddev>
              <bias_mean>0.1</bias_mean>
              <bias_stddev>0.001</bias_stddev>
            </noise>
          </x>
          <y>
            <noise type="gaussian">
              <mean>0.0</mean>
              <stddev>1.7e-2</stddev>
              <bias_mean>0.1</bias_mean>
              <bias_stddev>0.001</bias_stddev>
            </noise>
          </y>
          <z>
            <noise type="gaussian">
              <mean>0.0</mean>
              <stddev>1.7e-2</stddev>
              <bias_mean>0.1</bias_mean>
              <bias_stddev>0.001</bias_stddev>
            </noise>
          </z>
        </linear_acceleration>
      </imu>
    </sensor>
  </gazebo>

  <gazebo reference="laser_link">
    <sensor name="laser_sensor" type="ray">
      <always_on>true</always_on>
      <visualize>true</visualize>
      <update_rate>5</update_rate>
      <pose>0 0 0.075 0 0 0</pose>
      <ray>
        <scan>
          <horizontal>
            <samples>360</samples>
            <resolution>1.000000</resolution>
            <min_angle>0.000000</min_angle>
            <max_angle>6.280000</max_angle>
          </horizontal>
        </scan>
        <range>
          <min>0.120000</min>
          <max>3.5</max>
          <resolution>0.015000</resolution>
        </range>
        <noise>
          <type>gaussian</type>
          <mean>0.0</mean>
          <stddev>0.01</stddev>
        </noise>
      </ray>
      <plugin name="laserscan" filename="libgazebo_ros_ray_sensor.so">
        <ros>
          <!-- <namespace>/tb3</namespace> -->
          <remapping>~/out:=scan</remapping>
        </ros>
        <output_type>sensor_msgs/LaserScan</output_type>
        <frame_name>laser_link</frame_name>
      </plugin>
    </sensor>
  </gazebo>

</robot>
```



#### 2.6.1 gazebo例程

安装ros2&gazebo之后，我们可以在`/opt/ros/humble/share/gazebo_plugins/worlds`下看到gazebo提供的仿真地图。


比如我们打开仿真的小车，可以使用以下命令：

```sh
gazebo --verbose /opt/ros/humble/share/gazebo_plugins/worlds/gazebo_ros_diff_drive_demo.world
```

想要看看系统有没有跑起来，关键是看话题有没有发布/订阅：

```sh
ros2 topic list

# 如果看到以下话题就表示系统正常启动
...
/demo/cmd_demo
/demo/odom_demo
...
```

可以看到，其中有一个cmd_demo的话题，发布该话题就可以让gazebo中的小车模型动起来了：

```sh
ros2 topic pub /demo/cmd_demo geometry_msgs/Twist '{linear: {x: 1.0}}' -1
```

话题列表中还有一个odom_demo，可以实时反馈小车的里程计信息。

通过键盘控制小车移动：

```sh
# 安装键盘控制工具
sudo apt install ros-humble-teleop-twist-keyboard

# 这个功能包下有一个节点，这个节点会监听键盘的按键事件，然后发布cmd_vel话题，该话题被gazebo的两轮差速插件所订阅
ros2 run teleop_twist_keyboard teleop_twist_keyboard

# 如果你想让这个节点不是发布cmd_vel话题，而是别的，可以采用ROS2的话题重映射功能。 
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args --remap cmd_vel:=cmd_vel1

# 键盘指令
Moving around:
   u    i    o
   j    k    l
   m    ,    .
```

teleop_twist_keyboard键盘指令详解：

| 键盘 | 指令详解     |
| ---- | ------------ |
| u    | 向左前方前进 |
| i    | 直行前进     |
| o    | 向右前方前进 |
| j    | 逆时针旋转   |
| k    | 停止         |
| l    | 顺时针旋转   |
| m    | 向左后方后退 |
| ,    | 后退         |
| .    | 向右后方后退 |
| q    | 增加速度     |
| z    | 减小速度     |



在rviz中显示机器人和移动轨迹：

1. 修改FixedFrame为odom；
2. 添加插件，Add->Odometry->OK；
3. 选择话题，Odometry->Topic->选/odom；
4. 去除协方差显示，Odometry->Covariance>取消勾选；
5. 键盘控制节点，点个U，原地转圈圈；
6. 添加RobotModel插件，修改topic选中robot_description；



#### 2.6.2 探究仿真背后的内容

gazebo_ros_diff_drive_demo.world文件件内容并不算多，主要是[sdf模型](https://zhida.zhihu.com/search?content_id=100215725&content_type=Article&match_order=1&q=sdf模型&zhida_source=entity)文件的描述，头部注释里是该例程的使用方法：

```xml
Gazebo ROS differential drive plugin demo
 Try sending commands:
   ros2 topic pub /demo/cmd_demo geometry_msgs/Twist '{linear: {x: 1.0}}' -1
   ros2 topic pub /demo/cmd_demo geometry_msgs/Twist '{angular: {z: 0.1}}' -1
 Try listening to odometry:
   ros2 topic echo /demo/odom_demo
 Try listening to TF:
   ros2 run tf2_ros tf2_echo odom chassis
   ros2 run tf2_ros tf2_echo chassis right_wheel
   ros2 run tf2_ros tf2_echo chassis left_wheel
```

接下来是sdf模型相关的描述，除了ground和sun模型的调用外，重点是小车模型vehicle的描述，和[URDF建模](https://zhida.zhihu.com/search?content_id=100215725&content_type=Article&match_order=1&q=URDF建模&zhida_source=entity)的语法差别不大：

```xml
<model name='vehicle'>
     <pose>0 0 0.325 0 -0 0</pose>

     <link name='chassis'>
       <pose>-0.151427 -0 0.175 0 -0 0</pose>
       <inertial>
         <mass>1.14395</mass>
         <inertia>
           <ixx>0.126164</ixx>
           <ixy>0</ixy>
           <ixz>0</ixz>
           <iyy>0.416519</iyy>
           <iyz>0</iyz>
           <izz>0.481014</izz>
         </inertia>
       </inertial>
       <visual name='visual'>
         <geometry>
           <box>
             <size>2.01142 1 0.568726</size>
           </box>
         </geometry>
       </visual>
       <collision name='collision'>
         <geometry>
           <box>
             <size>2.01142 1 0.568726</size>
           </box>
         </geometry>
       </collision>
     </link>

     <link name='left_wheel'>
       <pose>0.554283 0.625029 -0.025 -1.5707 0 0</pose>
       <inertial>
         <mass>2</mass>
         <inertia>
           <ixx>0.145833</ixx>
           <ixy>0</ixy>
           <ixz>0</ixz>
           <iyy>0.145833</iyy>
           <iyz>0</iyz>
           <izz>0.125</izz>
         </inertia>
       </inertial>
       <visual name='visual'>
         <geometry>
           <sphere>
             <radius>0.3</radius>
           </sphere>
         </geometry>
       </visual>
       <collision name='collision'>
         <geometry>
           <sphere>
             <radius>0.3</radius>
           </sphere>
         </geometry>
         <surface>
           <friction>
             <ode>
               <mu>1</mu>
               <mu2>1</mu2>
               <slip1>0</slip1>
               <slip2>0</slip2>
             </ode>
           </friction>
           <contact>
             <ode>
               <soft_cfm>0</soft_cfm>
               <soft_erp>0.2</soft_erp>
               <kp>1e+13</kp>
               <kd>1</kd>
               <max_vel>0.01</max_vel>
               <min_depth>0.01</min_depth>
             </ode>
           </contact>
         </surface>
       </collision>
     </link>

     <link name='right_wheel'>
       <pose>0.554282 -0.625029 -0.025 -1.5707 0 0</pose>
       <inertial>
         <mass>2</mass>
         <inertia>
           <ixx>0.145833</ixx>
           <ixy>0</ixy>
           <ixz>0</ixz>
           <iyy>0.145833</iyy>
           <iyz>0</iyz>
           <izz>0.125</izz>
         </inertia>
       </inertial>
       <visual name='visual'>
         <geometry>
           <sphere>
             <radius>0.3</radius>
           </sphere>
         </geometry>
       </visual>
       <collision name='collision'>
         <geometry>
           <sphere>
             <radius>0.3</radius>
           </sphere>
         </geometry>
         <surface>
           <friction>
             <ode>
               <mu>1</mu>
               <mu2>1</mu2>
               <slip1>0</slip1>
               <slip2>0</slip2>
             </ode>
           </friction>
           <contact>
             <ode>
               <soft_cfm>0</soft_cfm>
               <soft_erp>0.2</soft_erp>
               <kp>1e+13</kp>
               <kd>1</kd>
               <max_vel>0.01</max_vel>
               <min_depth>0.01</min_depth>
             </ode>
           </contact>
         </surface>
       </collision>
     </link>

     <link name='caster'>
       <pose>-0.957138 -0 -0.125 0 -0 0</pose>
       <inertial>
         <mass>1</mass>
         <inertia>
           <ixx>0.1</ixx>
           <ixy>0</ixy>
           <ixz>0</ixz>
           <iyy>0.1</iyy>
           <iyz>0</iyz>
           <izz>0.1</izz>
         </inertia>
       </inertial>
       <visual name='visual'>
         <geometry>
           <sphere>
             <radius>0.2</radius>
           </sphere>
         </geometry>
       </visual>
       <collision name='collision'>
         <geometry>
           <sphere>
             <radius>0.2</radius>
           </sphere>
         </geometry>
       </collision>
     </link>

     <joint name='left_wheel_joint' type='revolute'>
       <parent>chassis</parent>
       <child>left_wheel</child>
       <axis>
         <xyz>0 0 1</xyz>
         <limit>
           <lower>-1.79769e+308</lower>
           <upper>1.79769e+308</upper>
         </limit>
       </axis>
     </joint>

     <joint name='right_wheel_joint' type='revolute'>
       <parent>chassis</parent>
       <child>right_wheel</child>
       <axis>
         <xyz>0 0 1</xyz>
         <limit>
           <lower>-1.79769e+308</lower>
           <upper>1.79769e+308</upper>
         </limit>
       </axis>
     </joint>

     <joint name='caster_wheel' type='ball'>
       <parent>chassis</parent>
       <child>caster</child>
     </joint>
```

重点是最下边的[差速控制器插件](https://zhida.zhihu.com/search?content_id=100215725&content_type=Article&match_order=1&q=差速控制器插件&zhida_source=entity)配置：

```xml
<plugin name='diff_drive' filename='libgazebo_ros_diff_drive.so'>

 <ros>
   <namespace>/demo</namespace>
   <argument>cmd_vel:=cmd_demo</argument>
   <argument>odom:=odom_demo</argument>
 </ros>

 <!-- wheels -->
 <left_joint>left_wheel_joint</left_joint>
 <right_joint>right_wheel_joint</right_joint>

 <!-- kinematics -->
 <wheel_separation>1.25</wheel_separation>
 <wheel_diameter>0.6</wheel_diameter>

 <!-- limits -->
 <max_wheel_torque>20</max_wheel_torque>
 <max_wheel_acceleration>1.0</max_wheel_acceleration>

 <!-- output -->
 <publish_odom>true</publish_odom>
 <publish_odom_tf>true</publish_odom_tf>
 <publish_wheel_tf>true</publish_wheel_tf>

 <odometry_frame>odom_demo</odometry_frame>
 <robot_base_frame>chassis</robot_base_frame>

</plugin>
```



### 2.7 [Gazebo仿真环境搭建](https://fishros.com/d2lros2/#/humble/chapt9/get_started/7.Gazebo仿真环境搭建?id=_7gazebo仿真环境搭建)

#### 2.7.1 Gazebo的world介绍

gazebo的world文件就是用于描述世界模型的，也就是环境模型。

Gazebo已经为我们准备了很多常用的物体模型，除了基础的圆球，圆柱，立方体外的。

但是一开始安装Gazebo的时候并不会帮你下载好这些模型，需要我们手动下载，通过下面的一行代码执行模型下载：

```sh
cd ~/.gazebo && wget https://gitee.com/ohhuo/scripts/raw/master/gazebo_model.py && python3 gazebo_model.py
```

然后等待脚本运行完成，当然也不用等它完成，因为一共有281个模型，是逐一下载并解压到`~/.gazebo/models/`目录的。

此时再次打开终端，输入`gazebo`，把选项卡切换到Insert，你就可以看到下载好的模型。

#### 2.7.2 通过Building Editor工具建立world

Gazebo左上角->Edit->Building Editor

接着可以看到这样一个编辑界面，通过选中wall，在view面板绘制墙体，下方scene页面就可以看到我们建好的world。

我们也可以通过一个绘制好的图片建墙，打开Gazebo->Gazebo左上角->Edit->Building Editor->左下方选Import，选中图片，然后导入，根据导入的图片边框绘制墙体。

**注意：导入完图片不会直接出来墙，图片只是提供了墙的大概位置，需要你手动用墙再将边描一遍。**

建完后点击File->Exit，在退出的弹框中选Exit，不要直接关掉编辑器，否则建立的world数据会丢失。

接着在Gazebo界面中就可以看到墙了，我们再手动添加几个物体，就可以用于下面的导航使用了。

添加完，接着点击File->SaveWorld，将文件保存到我们的fishbot_descrption的world下。

#### 2.7.3 启动时加载world

命令行加载：

```sh
gazebo --verbose  -s libgazebo_ros_init.so -s  libgazebo_ros_factory.so your_world_path/fishbot.world
```

在launch中加载：

```python
gazebo_world_path = os.path.join(pkg_share, 'world/fishbot.world')

# Start Gazebo server
start_gazebo_cmd =  ExecuteProcess(
    cmd=['gazebo', '--verbose','-s', 'libgazebo_ros_init.so', '-s', 'libgazebo_ros_factory.so', gazebo_world_path],
    output='screen')
```

最后记得修改setup.py文件，让编译后将world文件拷贝到install目录下，添加一行

```python
        (os.path.join('share', package_name, 'world'), glob('world/**')),
```

添加完后

```python
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        (os.path.join('share', package_name, 'launch'), glob('launch/*.launch.py')),
        (os.path.join('share', package_name, 'urdf'), glob('urdf/**')),
        (os.path.join('share', package_name, 'world'), glob('world/**')),
    ],

```



### 2.8 Setup Assistant工具

```sh
# 启动
ros2 launch moveit_setup_assistant setup_assistant.launch.py

# 通过该工具可以配置机械臂、末端夹爪等，RM的rm_moveit2_config就是通过该工具配置生成的
# 相机感知都是可以通过这个工具进行配置的
```

![SetupAssistent工具](./img/SetupAssistent.png)



### 2.9 ROS跨机器共享

[4.ROS2节点发现与多机通信](https://fishros.com/d2lros2/#/humble/chapt2/advanced/4.ROS2节点发现与多机通信?id=_4ros2节点发现与多机通信)

- 确保所有机器在同一网络中

  跨机器通信的前提是所有参与通信的机器需要在同一个网络中。确保它们可以相互访问，且没有防火墙等限制影响通信。

- 设置 **ROS_DOMAIN_ID**：

  在 ROS 2 中，所有通信都通过一个 **Domain ID** 进行隔离。默认情况下，ROS 2 会使用 `0` 作为 **Domain ID**，这意味着默认情况下，所有机器都在同一个 **Domain** 下进行通信。

  如果你有多个 ROS 2 系统在同一网络上，并且不希望它们的主题干扰，你可以设置不同的 **Domain ID**。但是，如果你希望它们共享主题，你需要确保所有机器使用相同的 **Domain ID**。

  在每台机器上设置相同的 `ROS_DOMAIN_ID`，例如：

  ```sh
  export ROS_DOMAIN_ID=24
  ```

- 使用ROS2 命令验证跨机器通信

  假设你已经设置好环境变量，确保所有机器在同一网络中，且配置了相同的 `ROS_DOMAIN_ID`。你可以使用 `ros2 topic list` 来验证主题是否能够跨机器共享。在每台机器上分别运行：

  ```sh
  ros2 topic list
  ```

  如果一切配置正确，你应该能够在不同的机器上看到共享的主题。

- 跨机器发布和订阅

  - 发布者机器

    ```sh
    ros2 run cpp_pubsub mytalker
    ```

  - 订阅者机器

    ```sh
    ros2 run cpp_pubsub mylistener
    ```

  如果配置正确，你将在订阅者机器上看到发布者机器发出的数据。

- 设置指定节点数据在某个域共享：这样可以设置如多机器人共享slam数据，然后本机互联的PC1，PC2共享摄像头，传感器数据等

  ```sh
  # 先设定ROS_DOMAIN_ID
  export ROS_DOMAIN_ID=24
  
  ros2 run cpp_pubsub mytalker
  
  # 取消ROS_DOMAIN_ID
  unset ROS_DOMAIN_ID
  ```



### 2.10 [ROS2帮助文档](https://docs.ros2.org/latest/api/rclcpp/index.html)

https://docs.ros.org/en/humble/

http://wiki.ros.org/message_filters

http://wiki.ros.org/cn

http://wiki.ros.org/APIs



## 3. ROS2示例

### 3.1 编写发布订阅包：`cpp_pubsub`

[编写简单的发布者和订阅者（C++）](https://docs.ros.org/en/rolling/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html)

- 导航到`ros2_ws/src`，并运行包创建命令

  ```sh
  ros2 pkg create --build-type ament_cmake --license Apache-2.0 cpp_pubsub
  
  # dependencies会自动向package.xml和CMakeLists.txt添加必要的依赖行
  ros2 pkg create --build-type ament_cmake --license Apache-2.0 --dependencies rclcpp std_msgs
  ```

- 编写发布&订阅节点

  进入`src/cpp_pubsub`目录

  - 编写发布节点：

    ```sh
    wget -O publisher_lambda_function.cpp https://raw.githubusercontent.com/ros2/examples/rolling/rclcpp/topics/minimal_publisher/lambda.cpp
    ```

    或直接使用vim创建publisher_lambda_function.cpp文件拷贝以下内容；

    ```c++
    #include <chrono>
    #include <memory>
    #include <string>
    
    #include "rclcpp/rclcpp.hpp"
    #include "std_msgs/msg/string.hpp"
    
    using namespace std::chrono_literals;
    
    /* This example creates a subclass of Node and uses a fancy C++11 lambda
    * function to shorten the callback syntax, at the expense of making the
    * code somewhat more difficult to understand at first glance. */
    
    class MinimalPublisher : public rclcpp::Node
    {
    public:
      MinimalPublisher()
      : Node("minimal_publisher"), count_(0)
      {
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
        auto timer_callback =
          [this]() -> void {
            auto message = std_msgs::msg::String();
            message.data = "Hello, world! " + std::to_string(this->count_++);
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
            this->publisher_->publish(message);
          };
        timer_ = this->create_wall_timer(500ms, timer_callback);
      }
    
    private:
      rclcpp::TimerBase::SharedPtr timer_;
      rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
      size_t count_;
    };
    
    int main(int argc, char * argv[])
    {
      rclcpp::init(argc, argv);
      rclcpp::spin(std::make_shared<MinimalPublisher>());
      rclcpp::shutdown();
      return 0;
    }
    ```

  - 编写订阅节点：

    ```sh
    wget -O subscriber_lambda_function.cpp https://raw.githubusercontent.com/ros2/examples/rolling/rclcpp/topics/minimal_subscriber/lambda.cpp
    ```

    或直接使用vim创建subscriber_lambda_function.cpp文件拷贝以下内容；

    ```c++
    #include <memory>
    
    #include "rclcpp/rclcpp.hpp"
    #include "std_msgs/msg/string.hpp"
    
    class MinimalSubscriber : public rclcpp::Node
    {
    public:
      MinimalSubscriber()
      : Node("minimal_subscriber")
      {
        auto topic_callback =
          [this](std_msgs::msg::String::UniquePtr msg) -> void {
            RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg->data.c_str());
          };
        subscription_ =
          this->create_subscription<std_msgs::msg::String>("topic", 10, topic_callback);
      }
    
    private:
      rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
    };
    
    int main(int argc, char * argv[])
    {
      rclcpp::init(argc, argv);
      rclcpp::spin(std::make_shared<MinimalSubscriber>());
      rclcpp::shutdown();
      return 0;
    }
    ```

    

- 在package.xml中添加依赖项

  - 填写包描述等信息

    ```xml
    <description>Examples of minimal publisher/subscriber using rclcpp</description>
    <maintainer email="you@email.com">Your Name</maintainer>
    <license>Apache-2.0</license>
    ```

  - 添加依赖项：在`ament_cmake` buildtool依赖项之后添加一个新行，并粘贴以下与节点的include语句对应的依赖项：

    ```xml
    <depend>rclcpp</depend>
    <depend>std_msgs</depend>
    ```

  - 完整信息录入参考

    ```xml
    <?xml version="1.0"?>
    <?xml-model href="http://download.ros.org/schema/package_format3.xsd" schematypens="http://www.w3.org/2001/XMLSchema"?>
    <package format="3">
      <name>cpp_pubsub</name>
      <version>0.0.0</version>
      <description>测试ros2节点的pub和sub功能</description>
      <maintainer email="robot@todo.todo">robot</maintainer>
      <license>Apache-2.0</license>
    
      <buildtool_depend>ament_cmake</buildtool_depend>
    
      <test_depend>ament_lint_auto</test_depend>
      <test_depend>ament_lint_common</test_depend>
    
      <export>
        <build_type>ament_cmake</build_type>
      </export>
    
      <depend>rclcpp</depend>
      <depend>std_msgs</depend>
    </package>
    ```

- CMakeLists.txt添加`rclcpp,std_msgs`；

  ```cmake
  cmake_minimum_required(VERSION 3.8)
  project(cpp_pubsub)
  
  if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
  endif()
  
  if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
  endif()
  
  # find dependencies
  find_package(ament_cmake REQUIRED)
  find_package(rclcpp REQUIRED)
  find_package(std_msgs REQUIRED)
  
  if(BUILD_TESTING)
    find_package(ament_lint_auto REQUIRED)
    # the following line skips the linter which checks for copyrights
    # comment the line when a copyright and license is added to all source files
    set(ament_cmake_copyright_FOUND TRUE)
    # the following line skips cpplint (only works in a git repo)
    # comment the line when this package is in a git repo and when
    # a copyright and license is added to all source files
    set(ament_cmake_cpplint_FOUND TRUE)
    ament_lint_auto_find_test_dependencies()
  endif()
  
  add_executable(talker src/publisher_lambda_function.cpp)
  ament_target_dependencies(talker rclcpp std_msgs)
  
  add_executable(listener src/subscriber_lambda_function.cpp)
  ament_target_dependencies(listener rclcpp std_msgs)
  
  install(TARGETS
    talker
    listener
    DESTINATION lib/${PROJECT_NAME})
  
  ament_package()
  ```

- 构建包&运行

  - 您可能已经安装了`rclcpp`和`std_msgs`包作为ROS 2系统的一部分。在构建之前，在工作区的根目录（`rosdep`）中运行`ros2_ws`来检查缺少的依赖项是一个很好的做法（这一步我执行失败了，但是后续步骤是ok的，所以暂时忽略它）：

    ```sh
    rosdep install -i --from-path src --rosdistro rolling -y
    ```

  - 仍然在工作区的根目录`ros2_ws`中，构建新的包：

    ```sh
    # 编译指定的包
    colcon build --packages-select cpp_pubsub
    
    # 编译所有包
    colcon build
    ```

    耐心等待编译。。。

  - 打开一个新的终端，导航到`ros2_ws`，然后获取安装文件：（终端关闭之后，需要重新执行）

    ```sh
    # .为source命令的简写
    . install/setup.bash
    ```

  - 打开2个终端，再次从`ros2_ws`内部获取设置文件，然后启动侦听器节点和发布节点：

    ```
    # 发布节点：这里更建议将cmake中编译的发布节点名修改为mytalker
    ros2 run cpp_pubsub talker
    
    # 侦听器节点：这里更建议将cmake中编译的侦听节点名修改为mylistener
    ros2 run cpp_pubsub listener
    ```

- 总结：创建了两个节点来发布和订阅主题上的数据。在编译和运行它们之前，将它们的依赖项和可执行文件添加到包配置文件中。



### 3.2 编写动作服务器&客户端（RPC）

[fishros：ROS2服务入门](https://fishros.com/d2lros2/#/humble/chapt3/get_started/4.ROS2服务入门?id=_4ros2服务入门)

[fishros：服务之RCLCPP实现](https://fishros.com/d2lros2/#/humble/chapt3/get_started/5.服务之RCLCPP实现?id=_5服务之rclcpp实现)

[用C++实现一个动作服务器和客户端](https://docs.ros.org/en/rolling/Tutorials/Intermediate/Writing-an-Action-Server-Client/Cpp.html)

服务的常用命令：

```sh
# 1. 启动服务
ros2 run examples_rclpy_minimal_service service

# 2. 查看服务列表
ros2 service list

# 3. 手动调用服务
ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts "{a: 5,b: 10}"

# 3.1 如果不写参数值调用会怎么样？实际测试结果显示，不填参数服务按默认值处理（目前还不确定是ros2系统是这么处理的，还是服务是这么实现的）
ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts

# 4. 查看服务接口类型，输出结果：example_interfaces/srv/AddTwoInts
ros2 service type /add_two_ints

# 5. 查找使用某一接口的服务，这一命令看起来和4刚好相反
ros2 service find example_interfaces/srv/AddTwoInts
```

导航到ros2_ws/src中，创建包：

```sh
ros2 pkg create demo_service --build-type ament_cmake --dependencies rclcpp

touch demo_service/src/demo_server.cpp
touch demo_service/src/demo_client.cpp
```

- 创建服务器&客户端

  - 服务器

    ```c++
    #include "rclcpp/rclcpp.hpp"
    
    #include "example_interfaces/srv/add_two_ints.hpp"
    using service_type_t = example_interfaces::srv::AddTwoInts;
    
    class DemoServer : public rclcpp::Node {
    public:
      DemoServer(std::string name) : Node(name) {
        RCLCPP_INFO(this->get_logger(), "节点已启动：%s.", name.c_str());
    
        add_ints_server_ = create_service<service_type_t>("add_two_ints_srv",
            [this](const std::shared_ptr<service_type_t::Request> request,
               std::shared_ptr<service_type_t::Response> response) {
          RCLCPP_INFO(this->get_logger(), "收到->a: %ld b: %ld", request->a, request->b);
          response->sum = request->a + request->b;
        });
      }
    
    private:
      rclcpp::Service<service_type_t>::SharedPtr add_ints_server_;
    };
    
    int main(int argc, char** argv) {
      rclcpp::init(argc, argv);
      rclcpp::spin(std::make_shared<DemoServer>("DemoServer"));
      rclcpp::shutdown();
      return 0;
    }
    ```

    

  - 客户端：注意多线程执行器

    ```c++
    #include "rclcpp/rclcpp.hpp"
    
    #include "example_interfaces/srv/add_two_ints.hpp"
    using service_type_t = example_interfaces::srv::AddTwoInts;
    
    class DemoClient : public rclcpp::Node {
    public:
      // 构造函数,有一个参数为节点名称
      DemoClient(std::string name) : Node(name) {
        RCLCPP_INFO(this->get_logger(), "节点已启动：%s.", name.c_str());
    
        client_ = this->create_client<service_type_t>("add_two_ints_srv");
      }
    
      void send_request(int a, int b) {
        std::stringstream ss;
        ss << "计算: " << a << " + " << b;
        auto req_str = ss.str();
    
        RCLCPP_INFO(this->get_logger(), req_str.c_str());
    
        // 1.等待服务端上线
        while (!client_->wait_for_service(std::chrono::seconds(1))) {
          //等待时检测rclcpp的状态
          if (!rclcpp::ok()) {
            RCLCPP_ERROR(this->get_logger(), "等待服务的过程中被打断...");
            return;
          }
          RCLCPP_INFO(this->get_logger(), ("等待服务端上线中: " + req_str).c_str());
        }
    
        // 2.构造请求的
        auto request = std::make_shared<service_type_t::Request>();
        request->a = a;
        request->b = b;
    
        // 3.发送异步请求，然后等待返回，返回时调用回调函数
        client_->async_send_request(request, [this, req_str](rclcpp::Client<service_type_t>::SharedFuture fut) {
            auto resp = fut.get();
            auto res_str = req_str + " = " + std::to_string(resp->sum);
            RCLCPP_INFO(this->get_logger(), res_str.c_str());
        });
      };
    
    private:
      rclcpp::Client<service_type_t>::SharedPtr client_;
    };
    
    int main(int argc, char** argv) {
      rclcpp::init(argc, argv);
    
    //   // 多线程执行器
    //   rclcpp::executors::MultiThreadedExecutor executor;
    
    //   auto cli = std::make_shared<DemoClient>("DemoClient01");
    //   executor.add_node(cli);
    
    //   auto cli2 = std::make_shared<DemoClient>("DemoClient02");
    //   executor.add_node(cli2);
    
    //   std::thread t1([cli](){
    //     cli->send_request(5, 6);
    //   });
    //   std::thread t2([cli2](){
    //     cli2->send_request(7, 8);
    //   });
    
    //   executor.spin();
    
    //   if(t1.joinable()) t1.join();
    //   if(t2.joinable()) t2.join();
    
      auto cli = std::make_shared<DemoClient>("DemoClient");
      cli->send_request(5, 6);
      rclcpp::spin(cli);
    
      rclcpp::shutdown();
      return 0;
    }
    ```

  - CMakeLists.txt

    这里我们一次性把服务端和客户端对example_interfaces的依赖都加上
    
    ```cmake
    find_package(example_interfaces REQUIRED)
    
    add_executable(DemoClient src/demo_client.cpp)
    ament_target_dependencies(DemoClient rclcpp example_interfaces)
    
    add_executable(DemoServer src/demo_server.cpp)
    ament_target_dependencies(DemoServer rclcpp example_interfaces)
    
    install(TARGETS
      DemoServer
      DESTINATION lib/${PROJECT_NAME}
    )
    
    install(TARGETS
      DemoClient
      DESTINATION lib/${PROJECT_NAME}
    )
    ```
  
  - packages.xml
  
    ```xml
    <depend>example_interfaces</depend>
    ```
  
    
  
  - 服务&客户端启动使用ros2 run即可；
  
  

### 3.3 在QT中订阅/发布ROS2主题

以QT程序集成ROS2环境，读取由ROS2节点 [realsense摄像头](https://github.com/IntelRealSense/realsense-ros?tab=readme-ov-file) 发布的数据为例；

```c++
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/image.hpp"

// 全局对象，放在cpp文件中
MainWindow* gApp = nullptr;

class MyRealsense : public rclcpp::Node
{
public:
  MyRealsense() : rclcpp::Node("MyRealsense")
  {
      // 主题可通过`ros2 topic list`进行查看
      subscription_ =
      this->create_subscription<sensor_msgs::msg::Image>("/camera/camera/color/image_raw", 10,
      [this](sensor_msgs::msg::Image::UniquePtr msg) {
        RCLCPP_INFO(this->get_logger(), "recv image, w: %d, h: %d", msg->width, msg->height);
        emit gApp->send_image(QImage(msg->data.data(), msg->width, msg->height, msg->step, QImage::Format_RGB888));
      });
  }

private:
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr subscription_;
};

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget *parent = nullptr)
    {
        gApp = this;
        
        QObject::connect(this, &MainWindow::send_refresh_image,
                         this, &MainWindow::recv_refresh_image);
        
        std::thread([]() {
            rclcpp::spin(std::make_shared<MyRealsense>());
        }).detach();
	}
    
    void send_image(QImage img) {
        emit this->send_refresh_image(std::move(img));
    }
    
signals:
    void send_refresh_image(QImage img);
    
private slots:
    void recv_refresh_image(QImage img) {
        ui->cameraImg->setPixmap(QPixmap::fromImage(img));
    	ui->cameraImg->setScaledContents(true);
    }
}

// main.cpp
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
```



cmake示例（**注意事项如下**）：

- ros2的依赖需要放入`ament_target_dependencies`中，且需要使用find_package查找包依赖；
- 非ros2的依赖放入`target_link_libraries中`；

```cmake
cmake_minimum_required(VERSION 3.5)

project(test LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

# ros packages.
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(ament_cmake REQUIRED)

add_executable(${PROJECT_NAME}
  main.cpp
  mainwindow.cpp
  mainwindow.h
  mainwindow.ui
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Widgets
)

# ros dependencies.
ament_target_dependencies(${PROJECT_NAME}
	rclcpp
    std_msgs
    sensor_msgs
)
```



### 3.4 ROS2接口

[7.ROS2接口介绍](https://fishros.com/d2lros2/#/humble/chapt3/get_started/7.ROS2接口介绍?id=_7ros2接口介绍)

接口相关命令

```sh
# 1. 查看某一接口包下所有的接口
ros2 interface package sensor_msgs

# 输出内容可能如下：
sensor_msgs/msg/JointState  #机器人关节数据
sensor_msgs/msg/Temperature #温度数据
sensor_msgs/msg/Imu #加速度传感器
sensor_msgs/msg/Image #图像
sensor_msgs/msg/LaserScan #雷达数据
......

# 2. 查看接口列表
ros2 interface list

# 3. 查看接口详情
ros2 interface show std_msgs/msg/String

# 用proto在看一下包含的基本数据类型有哪些
ros2 interface proto geometry_msgs/msg/Twist

# 4. 通过命令行调用接口([3.2 编写动作服务器&客户端（RPC）](#3.2 编写动作服务器&客户端（RPC）))
ros2 service call /add_two_ints example_interfaces/srv/AddTwoInts "{a: 5,b: 10}"
```



ROS2提供了四种通信方式：

- 话题-Topics
- 服务-Services
- 动作-Action
- 参数-Parameters

除了参数之外，话题、服务和动作(Action)都支持自定义接口，每一种通信方式所适用的场景各不相同，所定义的接口也被分为话题接口、服务接口、动作接口三种。

#### 3.4.1 接口形式

- 话题接口格式：`xxx.msg`

  ```
  int64 num
  ```

  

- 服务接口格式：`xxx.srv`

  ```
  int64 a
  int64 b
  ---
  int64 sum
  ```

  

- 动作接口格式：`xxx.action`

  ```
  int32 order
  ---
  int32[] sequence
  ---
  int32[] partial_sequence
  
  ```

  

#### 3.4.2 接口数据类型

```
bool
byte
char
float32,float64
int8,uint8
int16,uint16
int32,uint32
int64,uint64
string
```

包装类型：即在已有的接口类型上进行包含，比如：

```
uint32 id
string image_name
sensor_msgs/Image
```

接口代码生成：

`【msg,srv,action】=>【ROS2-IDL】=>【py，h】`

通过ROS2的 IDL 模块产生了头文件，有了头文件，我们就可以在程序里导入并使用这个消息模块。



#### 3.4.3 自定义接口实践

给定一个机器人开发中的常见控制场景，我们设计满足要求的服务接口和话题接口，设计两个节点：

- 一个机器人节点，对外提供移动指定距离服务，移动完成后返回当前位置，同时对外发布机器人的位置和状态（是否在移动）。
- 机器人控制节点，通过服务控制机器人移动指定距离，并实时获取机器人的当前位置和状态。

假设机器人在坐标轴上，只能前后移动。



##### 3.4.3.1 创建自定义接口

- 进入ros2_ws，创建自定义接口包：
  
```sh
  # rosidl_default_generators必要的依赖，geometry_msgs可选依赖
  ros2 pkg create demo_iface --build-type ament_cmake --dependencies rosidl_default_generators geometry_msgs
```

  

- 定义接口
  服务接口：`MoveRobot.srv`

  ```
  # 前进后退的距离
  float32 distance
  ---
  # 当前的位置
  float32 pose
  ```

  话题接口，采用基础类型 `RobotStatus.msg`

  ```
  uint32 STATUS_MOVEING = 1
  uint32 STATUS_STOP = 2
  uint32  status
  float32 pose
  ```

  话题接口，混合包装类型 `RobotPose.msg`

  ```
  uint32 STATUS_MOVEING = 1
  uint32 STATUS_STOP = 2
  uint32  status
  geometry_msgs/Pose pose
  ```

  接着创建文件夹和文件上述文件写入指定目录中，注意话题接口放到`msg`文件夹下，以`.msg`结尾。服务接口放到`srv`文件夹下，以`srv`结尾。

  ```
  .
  ├── CMakeLists.txt
  ├── msg
  │   ├── RobotPose.msg
  │   └── RobotStatus.msg
  ├── package.xml
  └── srv
      └── MoveRobot.srv
  
  2 directories, 5 files
  ```
  
  
  
- 接着修改`CMakeLists.txt`

  ```cmake
  find_package(rosidl_default_generators REQUIRED)
  find_package(geometry_msgs REQUIRED)
  
  # -----------添加下面的内容-----------
  rosidl_generate_interfaces(${PROJECT_NAME}
    "msg/RobotPose.msg"
    "msg/RobotStatus.msg"
    "srv/MoveRobot.srv"
    DEPENDENCIES geometry_msgs
  )
  ```

  

- 接着修改`package.xml`

  ```xml
    <buildtool_depend>ament_cmake</buildtool_depend>
  
    <depend>rosidl_default_generators</depend>
    <depend>geometry_msgs</depend>
    
    <member_of_group>rosidl_interface_packages</member_of_group> #<----添加这一行
  
    <test_depend>ament_lint_auto</test_depend>
    <test_depend>ament_lint_common</test_depend>
  ```

  

- 保存并编译

  ```sh
  colcon build --packages-select demo_iface
  ```

  编译完成后在`ros2_ws/install/demo_iface/include`下你应该可以看到C++的头文件。

  在`ros2_ws/install/demo_iface/local/lib/python3.10/dist-packages`下应该可以看到Python版本的头文件。

  **@Note：**

  1. 如果没有在上述目录看到头文件，首先确认编译是否正常；

  2. 如果编译正常，仍然没有头文件，可能是未重新编译导致（和cmake的缓存有关），执行下面的命令，应该能看到头文件了；

     ```sh
     # 清理工作空间
     colcon build --symlink-install --cmake-target clean
     
     # 重新编译
     colcon build --symlink-install
     ```

     

##### 3.4.3.2 引用自定义接口

[8.自定义接口RCLCPP实战](https://fishros.com/d2lros2/#/humble/chapt3/get_started/8.自定义接口RCLCPP实战?id=_8自定义接口rclcpp实战)

```sh
# demo_iface：为上一节创建的自定义接口/话题数据包
cd chapt3_ws/src
ros2 pkg create demo_iface_rclcpp --build-type ament_cmake --dependencies rclcpp demo_iface
touch demo_iface_rclcpp/src/demo_iface_control.cpp
touch demo_iface_rclcpp/src/demo_iface_robot.cpp
```

编写demo_iface_robot.cpp

```c++
#include "rclcpp/rclcpp.hpp"
#include "demo_iface/msg/robot_status.hpp"
#include "demo_iface/srv/move_robot.hpp"

class Robot {
public:
  Robot() = default;
  ~Robot() = default;
  /**
   * @brief 移动指定的距离
   *
   * @param distance
   * @return float
   */
  float move_distance(float distance) {
    status_ = demo_iface::msg::RobotStatus::STATUS_MOVEING;
    target_pose_ += distance;
    // 当目标距离和当前距离大于0.01则持续向目标移动
    while (fabs(target_pose_ - current_pose_) > 0.01) {
      // 每一步移动当前到目标距离的1/10
      float step = distance / fabs(distance) * fabs(target_pose_ - current_pose_) * 0.1;
      current_pose_ += step;
      std::cout << "移动了：" << step << "当前位置：" << current_pose_ << std::endl;
      // 当前线程休眠500ms
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    status_ = demo_iface::msg::RobotStatus::STATUS_STOP;
    return current_pose_;
  }
  /**
   * @brief Get the current pose
   *
   * @return float
   */
  float get_current_pose() { return current_pose_; }

  /**
   * @brief Get the status
   *
   * @return int
   *  1 demo_iface::msg::RobotStatus::STATUS_MOVEING
   *  2 demo_iface::msg::RobotStatus::STATUS_STOP
   */
  int get_status() { return status_; }

private:
  // 声明当前位置
  float current_pose_ = 0.0;
  // 目标距离
  float target_pose_ = 0.0;
  int status_ = demo_iface::msg::RobotStatus::STATUS_STOP;
};

class ExampleInterfacesRobot : public rclcpp::Node {
public:
  ExampleInterfacesRobot(std::string name) : Node(name) {
    RCLCPP_INFO(this->get_logger(), "节点已启动：%s.", name.c_str());

    /*创建move_robot服务*/
    move_robot_server_ = this->create_service<demo_iface::srv::MoveRobot>("move_robot",
      [this](const std::shared_ptr<demo_iface::srv::MoveRobot::Request> request,
             std::shared_ptr<demo_iface::srv::MoveRobot::Response> response) {
      RCLCPP_INFO(this->get_logger(), "收到请求移动距离：%f，当前位置：%f",
                  request->distance, robot_.get_current_pose());
      robot_.move_distance(request->distance);
      response->pose = robot_.get_current_pose();
    });
    
    /*创建发布者*/
    robot_status_publisher_ = this->create_publisher<demo_iface::msg::RobotStatus>("robot_status", 10);

    /*创建一个周期为500ms的定时器*/
    timer_ = this->create_wall_timer(std::chrono::milliseconds(500), [this](){
      // 创建消息
      demo_iface::msg::RobotStatus message;
      message.status = robot_.get_status();
      message.pose = robot_.get_current_pose();
      RCLCPP_INFO(this->get_logger(), "Publishing: %f", robot_.get_current_pose());

      // 发布消息
      robot_status_publisher_->publish(message);
    });
  }

private:
  Robot robot_; /*实例化机器人*/
  rclcpp::TimerBase::SharedPtr timer_; /*定时器，用于定时发布机器人位置*/
  rclcpp::Service<demo_iface::srv::MoveRobot>::SharedPtr move_robot_server_; /*移动机器人服务*/
  rclcpp::Publisher<demo_iface::msg::RobotStatus>::SharedPtr robot_status_publisher_; /*发布机器人位姿发布者*/
};


int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ExampleInterfacesRobot>("example_interfaces_robot_01");
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
```

编写demo_iface_control.cpp

```c++
#include "rclcpp/rclcpp.hpp"
#include "demo_iface/srv/move_robot.hpp"
#include "demo_iface/msg/robot_status.hpp"

class ExampleInterfacesControl : public rclcpp::Node {
public:
  ExampleInterfacesControl(std::string name) : Node(name) {
    RCLCPP_INFO(this->get_logger(), "节点已启动：%s.", name.c_str());
    /*创建move_robot客户端*/
    client_ = this->create_client<demo_iface::srv::MoveRobot>("move_robot");
    /*订阅机器人状态话题*/
    robot_status_subscribe_ =
     this->create_subscription<demo_iface::msg::RobotStatus>("robot_status", 10,
      [this](const demo_iface::msg::RobotStatus::SharedPtr msg) {
      RCLCPP_INFO(this->get_logger(), "收到状态数据位置：%f 状态：%d", msg->pose ,msg->status);
    });
  }


  /**
   * @brief 发送移动机器人请求函数
   * 步骤：1.等待服务上线
   *      2.构造发送请求
   * 
   * @param distance 
   */
  void move_robot(float distance) {
    RCLCPP_INFO(this->get_logger(), "请求让机器人移动%f", distance);

    /*等待服务端上线*/
    while (!client_->wait_for_service(std::chrono::seconds(1))) {
      //等待时检测rclcpp的状态
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(this->get_logger(), "等待服务的过程中被打断...");
        return;
      }
      RCLCPP_INFO(this->get_logger(), "等待服务端上线中");
    }

    // 构造请求
    auto request = 
      std::make_shared<demo_iface::srv::MoveRobot::Request>();
    request->distance = distance;

    // 发送异步请求，然后等待返回，返回时调用回调函数
    client_->async_send_request(
      request, [this](rclcpp::Client<demo_iface::srv::MoveRobot>::SharedFuture
      result_future) {
    auto response = result_future.get();
    RCLCPP_INFO(this->get_logger(), "收到移动结果：%f", response->pose);});
  };

private:
  // 声明客户端
  rclcpp::Client<demo_iface::srv::MoveRobot>::SharedPtr client_;
  rclcpp::Subscription<demo_iface::msg::RobotStatus>::SharedPtr robot_status_subscribe_;
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<ExampleInterfacesControl>("example_interfaces_control_01");
  /*这里调用了服务，让机器人向前移动5m*/
  node->move_robot(5.0);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
```

编译依赖&包：

```sh
# packages-up-to：先编译依赖（demo_iface）再编译demo_iface_rclcpp
colcon build --packages-up-to demo_iface_rclcpp
```



## 4. DDS相关

![ROS2架构](./img/ROS2架构.png)

[ROS2系统架构](https://fishros.com/d2lros2/#/humble/chapt1/advanced/1.ROS2系统架构?id=_1ros2系统架构)

[ROS2中间件DDS架构](https://fishros.com/d2lros2/#/humble/chapt1/advanced/2.中间件DDS架构?id=_2ros2中间件dds架构)

[DDS 发现](https://fishros.com/d2lros2/#/humble/codebook/pages/networking)

[HelloFish例程](https://fishros.com/d2lros2/#/humble/chapt3/advanced/4.使用DDS进行订阅发布?id=_3hellofish例程)

无需配置环境，ROS2集成了FastDDS，可以直接按照链接下载源码编译之后查看DDS效果。

如果想要获得更多关于DDS用法需要查阅 [FastDDS](https://github.com/eProsima/Fast-DDS)。



## 5. 开环控制与闭环控制

[1.开关控制与闭环控制](https://fishros.com/d2lros2/#/humble/chapt4/basic/1.开环控制与闭环控制?id=_1开关控制与闭环控制)

机器人的控制可以分为两种类型：开环控制和闭环控制。

### 5.1 开环控制介绍

开环控制是指机器人按照预先设定的命令执行任务，但并不会对执行过程中的状态进行反馈和调整。简单来说，开环控制就是机器人盲目地按照指令执行任务，不考虑实际执行情况是否符合预期。开环控制的优点是简单易用，适用于一些简单的任务，如基本的运动控制或简单的搬运。但是，它的缺点也很明显，因为机器人无法感知执行任务的实际情况，因此无法自动调整行动，导致执行任务的成功率低，可靠性差。

### 5.2 闭环控制介绍

闭环控制是指机器人通过传感器或其他检测设备获取执行任务过程中的状态信息，将这些信息反馈给控制系统，从而实现对机器人执行任务过程中的实时控制和调整。在闭环控制中，机器人执行任务的过程中会根据反馈信息调整执行动作，确保机器人按照预期的方式完成任务。闭环控制的优点是能够根据实际情况进行实时调整，提高了机器人执行任务的成功率和可靠性。

### 5.3 哪个更常用

在机器人应用中，闭环控制更加普遍，因为它能够根据反馈信息实时调整机器人的动作，确保机器人按照预期的方式执行任务。同时，闭环控制还可以帮助机器人适应不同的工作环境，增强机器人的鲁棒性和自适应能力，实现更高效、更精准的控制。在一些高精度和复杂的应用场景中，闭环控制已经成为机器人控制的标准，比如精密加工、医疗手术、自动驾驶等。

