# Rviz工程说明

以下是 `rviz_assimp_vendor`、`rviz_common`、`rviz_default_plugins`、`rviz_ogre_vendor`、`rviz_rendering`、`rviz_rendering_tests`、`rviz_visual_testing_framework` 和 `rviz2` 等 ROS 2 中 rviz 相关模块的功能说明：

------

### 📁 `rviz_assimp_vendor`

- **功能**：提供对 [Assimp](https://github.com/assimp/assimp)（Open Asset Import Library）的封装，用于加载 3D 模型文件（如 `.obj, .stl, .dae, .fbx` 等）。
- 用途：
  - 支持在 rviz 中显示复杂的 3D 模型；
  - 作为第三方依赖库集成到 rviz 渲染流程中；
  - 提供模型导入和转换为 OGRE 可用格式的能力。
- 关键内容：
  - 构建脚本（CMakeLists.txt）用于下载并构建 Assimp；
  - 将 Assimp 静态库或动态库链接到 `rviz_rendering`。

------

### 📁 `rviz_common`

- **功能**：包含 rviz 的核心非 UI 逻辑，与 Qt 无关的部分。
- 用途：
  - 定义可视化管理器 (`VisualizationManager`)、场景管理 (`SceneManager`)、属性系统 (`Property`)、插件接口等；
  - 提供跨平台基础功能（如时间、坐标变换、消息处理）；
  - 支持将 rviz 集成到其他应用程序中（不依赖 GUI）。
- 关键类：
  - `VisualizationManager`
  - `Display`
  - `Tool`
  - `PropertyTreeModel`
- **适用于**：开发无界面的可视化引擎或嵌入式使用。

------

### 📁 `rviz_default_plugins`

- **功能**：官方提供的默认显示插件集合。
- 用途：
  - 实现常用传感器数据的可视化（如激光雷达、点云、路径、地图等）；
  - 提供可扩展的插件结构供开发者参考；
  - 包含多个子目录，每个对应一个插件类型。
- 常见插件：
  - `Grid`：网格显示
  - `LaserScan`：激光雷达数据
  - `Map`：二维地图
  - `Path`：轨迹路径
  - `RobotModel`：机器人模型
  - `Marker`：自定义标记（支持多种形状）
- 依赖关系：
  - 使用 `pluginlib` 进行动态加载；
  - 依赖 `rviz_common` 和 `rviz_rendering`。

------

### 📁 `rviz_ogre_vendor`

- **功能**：打包并编译 [OGRE](https://github.com/OGRECave/ogre) 图形渲染引擎，作为 rviz 的底层图形库。
- 用途：
  - 提供跨平台的 3D 渲染能力；
  - 被 `rviz_rendering` 使用以实现所有图形绘制功能；
  - 包括必要的补丁（patch）和 CMake 构建配置。
- 关键操作：
  - 下载指定版本的 OGRE（如 v1.12.1）；
  - 构建为共享库（`.dll` / `.so` / `.dylib`）；
  - 应用补丁修复兼容性问题（如 Windows 编译、ARM64 支持等）；
  - 设置环境变量以确保运行时能找到 OGRE 库。
- 典型文件：
  - `CMakeLists.txt`：控制构建过程；
  - `pragma-patch.diff,fix-arm64.diff`：补丁文件；
  - `FindFreetype.cmake`：查找字体库。

------

### 📁 `rviz_rendering`

- **功能**：基于 OGRE 实现 rviz 的 3D 渲染逻辑。
- 用途：
  - 封装 OGRE 场景、相机、灯光、材质等；
  - 提供 `RenderPanel`控件用于嵌入 Qt 界面；
  - 实现几何体绘制、贴图加载、着色器管理等功能；
  - 是 `rviz_common`和 `rviz2` 的图形后端。
- 关键类：
  - `RenderWindow`
  - `SceneNode`
  - `Shape`
  - `BillboardLine`
  - `MovableText`
- 依赖关系：
  - 依赖`rviz_common`（核心逻辑）；
  - 依赖 `rviz_ogre_vendor`（图形引擎）；
  - 可选依赖 `rviz_assimp_vendor`（3D 模型加载）。

------

### 📁 `rviz_rendering_tests`

- **功能**：对 `rviz_rendering` 模块进行单元测试和渲染验证。
- 用途：
  - 测试基本图形渲染是否正确；
  - 验证光照、材质、相机视角等功能；
  - 确保不同平台下一致性；
  - 提供自动化测试框架。
- 典型内容：
  - 使用 `gtest` 编写测试用例；
  - 利用 `rviz_visual_testing_framework` 进行截图比对；
  - 包含资源目录（如模型、纹理等）用于测试。

------

### 📁 `rviz_visual_testing_framework`

- **功能**：提供可视化测试工具集，支持截图比对、录制回放等。
- 用途：
  - 自动化测试中判断图形输出是否符合预期；
  - 记录当前渲染画面并与基准图像对比；
  - 用于 CI/CD 流程中的回归测试。
- 关键特性：
  - `ScreenshotTester`：截图比对类；
  - `VisualTest`：测试基类；
  - 支持设置容忍误差范围（像素差异）；
  - 生成 HTML 报告展示测试结果。

------

### 📁 `rviz2`

- **功能**：rviz 的主应用程序入口，即最终用户运行的可视化工具。
- 用途：
  - 主程序启动界面；
  - 初始化窗口、菜单栏、工具条等；
  - 加载默认配置文件（如 `default.rviz`）；
  - 集成插件系统；
  - 启动 ROS 节点并订阅数据。
- 关键文件：
  - `main.cpp`：程序入口；
  - `MainWindow` 类：主窗口控制器；
  - `ui`：Qt UI 文件；
  - `plugins.xml`：插件注册文件；
  - `package.xml`：声明依赖项；
  - `launch/`：ROS 启动文件。

------

## ✅ 总结表格

| 文件夹                          | 功能                                 |
| :------------------------------ | :----------------------------------- |
| `rviz_assimp_vendor`            | 第三方 3D 模型加载库（Assimp）的封装 |
| `rviz_common`                   | 核心逻辑层（非 UI），插件系统基础    |
| `rviz_default_plugins`          | 官方默认显示插件集合                 |
| `rviz_ogre_vendor`              | 打包构建 OGRE 图形引擎               |
| `rviz_rendering`                | 基于 OGRE 的 3D 渲染实现             |
| `rviz_rendering_tests`          | 渲染相关单元测试                     |
| `rviz_visual_testing_framework` | 可视化测试框架（截图比对等）         |
| `rviz2`                         | 主程序入口，完整 GUI 应用            |

------

如果你计划进行 **rviz 插件开发** 或 **定制化集成**，建议重点关注以下模块：

- `rviz_common`：理解整体架构；
- `rviz_default_plugins`：学习插件编写方式；
- `rviz_rendering`：掌握图形渲染机制；
- `rviz2`：了解主程序如何整合各模块；
- `rviz_ogre_vendor`：若需修改图形底层行为。