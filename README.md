# ImageProcess
Wheat同学写的图像预处理算法工厂

项目维护一个图像预处理程序ImageProcess.exe，该程序具有以下特性：

- 程序包含带多个处理算法的工厂，处理算法区分“差异化处理”和“非差异化处理”方式，关于此处“处理算法”及“（非）差异化处理”的定义，查阅[ImageProcess文档.docx](https://github.com/Wheat2018/ImageProcess/blob/master/ImageProcess文档.docx)

- 读取某单个/多个目录下的、指定后缀名的全部图片，调用工厂中指定算法，处理结果生成到指定目录。

- 以上操作通过Windows控制台(CMD)带参调用ImageProcess.exe实现


ImageProcess项目具有以下特性：

- 定义了“处理算法”、“差异化处理算法”、“非差异化处理算法”，查阅[ImageProcess文档.docx](https://github.com/Wheat2018/ImageProcess/blob/master/ImageProcess文档.docx)

- 项目框架分别定义了Solve类、DiscSolve类、IndiscSolve类，后两者继承自前者，分别管理一个算法工厂（算法名称/算法构造器 键值对）。

- 所有差异化处理算法继承DiscSolve类，非差异化处理算法继承IndiscSolve类，每种最终算法类采用提供的宏模板定义，在宏模板中自动向算法工厂注册该类。

- 除discFactory和indiscFactory两种工厂外，还有summary管理器（算法名称/算法描述 键值对），为描述算法功能提供便利。

- 项目实现控制台(CMD)参数调用功能，依赖于[WheatAPI](https://github.com/Wheat2018/WheatAPI)中的ParameterManager类。

- 项目框架及实现细节参考[ImageProcess文档.docx](https://github.com/Wheat2018/ImageProcess/blob/master/ImageProcess文档.docx)
