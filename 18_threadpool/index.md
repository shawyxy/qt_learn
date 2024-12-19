线程池
1. 线程任务类继承QObject,用于使用信号槽机制;继承QRunnable,用于重写run方法(它是public)
2. 声明修改后,对应的实现的构造方法`QObject{parent}, QRunnable()`
3. 通过`setAutoDelete(true)`使任务对象在执行完毕后自动被销毁,这样当第二次点击开始按钮时,任务对象已经被销毁,程序会因非法内存访问而崩溃.

背后的原理是，Qt 的事件循环在一个特定的线程中运行，moveToThread() 会将任务对象的所有信号槽连接到目标线程的事件循环中。当线程启动时，事件循环开始运行，任务对象的槽函数就会在这个新的线程中执行。所以你需要先启动线程，让事件循环开始运行，然后再调用任务函数。通过连接 QThread::started 信号到任务函数，你确保了任务函数在事件循环已经开始运行时被调用。