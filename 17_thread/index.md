方法2:https://subingwen.cn/qt/thread/#3-%E4%BD%BF%E7%94%A8%E6%96%B9%E5%BC%8F2
方法1是用QThread的派生类重写run方法,执行线程任务,但是run方法的参数列表不能被修改,所以传参需要在类中定义成员变量.并且一个类只有一个任务函数
用OBject的派生类作为任务类,它是独立于任何类的存在,不在对象树中(内存回收?)
只保留信号的成员函数,删除用于参数传递的成员及run方法,线程任务执行函数的声明可以按需求指定,并且可以有多个.
将所有QThread改成QObject,QObject中有`moveToThread()`方法,自定义的任务类可以继承
一个线程可以通过`moveToThread()`方法移动多个对象,执行顺序和加入的顺序有关

在主线程中:
1. 创建任务对象
2. 创建线程对象
3. 任务对象指针->moveToThread,加入到线程中
4. 其他逻辑不变,需要注意connect函数中发起信号的主体不再是线程对象,而是任务对象

注意:
1. 不可以在主线程中直接调用任务对象的执行函数,即使在此之前已经调用过moveToThread,程序可以执行,但是违背了使用多线程的初衷.
2. 子线程对象的资源释放:
    - 通过`this`加入到对象树中,得是QObject的子类
    - 通过析构函数,但是需要将线程对象的指针加入到成员变量,不方便
    - 当窗口发出destroyed信号(当按下右上角的关闭按钮自动触发),手动依次调用每个线程的成员:quit,wait和deleteLater(封装了delete),任务对象则通过deleteLater释放资源(和new对应)