# ImageProcess
Wheat同学写的图像预处理算法工厂

1. [程序特性](#程序特性)
2. [项目特性](#项目特性)
3. [附言](#附言)

## 程序特性
项目维护一个图像预处理程序ImageProcess.exe，该程序具有以下特性：

- 程序包含带多个处理算法的工厂，处理算法区分“差异化处理”和“非差异化处理”方式，关于此处“处理算法”及“（非）差异化处理”的定义，查阅[ImageProcess文档.docx](https://github.com/Wheat2018/ImageProcess/blob/master/ImageProcess文档.docx)

- 读取某单个/多个目录下的、指定后缀名的全部图片，调用工厂中指定算法，处理结果生成到指定目录。

- 以上操作通过Windows控制台(CMD)带参调用ImageProcess.exe实现

## 项目特性
ImageProcess项目具有以下特性：

- 定义了“处理算法”、“差异化处理算法”、“非差异化处理算法”，查阅[ImageProcess文档.docx](https://github.com/Wheat2018/ImageProcess/blob/master/ImageProcess文档.docx)

- 项目框架分别定义了Solve类、DiscSolve类、IndiscSolve类，后两者继承自前者，分别管理一个算法工厂（算法名称/算法构造器 键值对）。

- 所有差异化处理算法继承DiscSolve类，非差异化处理算法继承IndiscSolve类，每种最终算法类采用提供的宏模板定义，在宏模板中自动向算法工厂注册该类。

- DiscSolve和IndiscSolve作为两种算法工厂，提供Create(std::string)->DiscSolve* 或 IndiscSolve* 构造器，由算法名称构造算法实例，两种工厂生成的实例用对应的父类指针绑定。

- 除DiscSolve和IndiscSolve两种工厂外，还有summary管理器（算法名称/算法描述 键值对），由Solve类提供AddSummary(string,string)->void描述添加器和Summary(string)->string描述查找器，为描述算法功能提供便利。

- 项目实现控制台(CMD)参数调用功能，依赖于[WheatAPI](https://github.com/Wheat2018/WheatAPI)中的ParameterManager类。

- 项目框架及实现细节参考[ImageProcess文档.docx](https://github.com/Wheat2018/ImageProcess/blob/master/ImageProcess文档.docx)

## 附言
- 该项目是参加2019年全国大学生计算机设计大赛（人工智能挑战赛-膀胱肿瘤分级分期标定）时开展的，用途是给膀胱肿瘤数据做数据增广。

- 客观地说，整个框架其实是为“非差异化处理算法”而生，如果只含非差异化处理算法，框架会非常明朗，添加新算法的开发成本也比较低。（非差异化处理算法亦即输入一张图片，得到一张图片的算法，诸如：直方图均衡、边界增强等等。在[ImageProcess文档.docx](https://github.com/Wheat2018/ImageProcess/blob/master/ImageProcess文档.docx)中有描述）那么批量处理，只需要给一个输入目录，一个输出目录，及算法的各种参数设定即可。

- 但数据增广常常用到的算法不止这种类型。举个例子，直方图均衡、边界增强等等算法，一来对输入的每张图都是同种处理手段，二来只需要应用到数据图上，而不需要应用到数据标签上。但类似于patch采样（用小框从图上挖个小图出来）、随机旋转、镜像翻转等等处理操作，一来对每张图处理不太一样，如旋转角度不同，二来需要将数据图和数据标签做同步处理的，这样与非差异化处理就需区分开来。差异化处理在框架中是定义需要给一个输入目录、标签目录，一个输出目录、输出标签目录，及算法的各种参数设定。

- 差异化算法是硬加进框架中的，整个框架或许没有很合理，也不是很巧妙。但使用工厂模式管理算法的思想具有一定的参考意义，当后续有任何新算法时，只需要使用提供的宏模板去声明算法类，在类的ImageSolve虚函数中填入算法内容，在程序全局变量初始化阶段即可在工厂中注册，开发者可以使用类似“反射”机制，通过算法名称获得该算法实例。
