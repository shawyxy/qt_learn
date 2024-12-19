0. 用组件创建多个子窗口
1. 创建继承QThread的子类, 选择QObject,在头文件和源文件中,把QObject改成QThread(除了构造函数的参数外,对象树)
2. 重写QThread的run保护函数,用于执行线程的任务
    0. 只有主线程可以对窗口进行读写操作
    1. 通过信号和槽的参数实现父子线程之间的数据传递
3. 线程1的任务是生成随机数,保存在QVector容器list中,最终线程通过自定义信号将list中的内容传递给主进程

随机数参数传递流程:
1. 主线程创建子线程,在启动之前需要指定随机数的个数,因此需要在主线程中定义一个sendNum的信号,connect(sendNum/*信号*/, recvNum/*槽*/).
2. 主线程通过按钮Clicked信号发送sendNum信号,并调用start启动子线程,子线程会收到参数并生成一个随机数列表.
3. 子线程定义sendList信号,主线程定义对应的槽函数recvList.

注意要用`qRegisterMetaType<QVector<int>>("QVector<int>")`传递容器参数

排序参数传递流程同上,需要注意的是别忘了在源文件中给排序线程写构造函数.

