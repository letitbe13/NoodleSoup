    #include <QCoreApplication>
    #include <unistd.h>
    #include <signal.h>
    #include <sys/stat.h>

    #include <test_server.h>

    using namespace std;

    static void prepare_service_mode()
    {
        pid_t pid;
        pid = fork();//для работы в фоне

        if (pid < 0) exit(EXIT_FAILURE);
        if (pid > 0) exit(EXIT_SUCCESS);

        if (setsid() < 0) exit(EXIT_FAILURE);

        //сервис не должен быть завершен по системным сигналам о
        signal(SIGCHLD, SIG_IGN); //о переменах в статусе дочернего процесса
        signal(SIGHUP, SIG_IGN); //закрытии "родительского" терминала
        //для полноценного управления сервисом по-хорошему нужно реализовать обработчики для системных сигналов.

        pid = fork();
        if (pid < 0) exit(EXIT_FAILURE);
        if (pid > 0) exit(EXIT_SUCCESS);

        umask(0);
//        chdir("/");
//        закрываем файлы, которые мог открыть родительский процесс
        int x;
        for (x = sysconf(_SC_OPEN_MAX); x>=0; x--)
        {
            close (x);
        }

    }

    int main(int argc, char * argv[])
    {

        prepare_service_mode();

        QCoreApplication a(argc,argv);

        int port;
        if(argc > 1)    port = QVariant(argv[1]).toInt();
        else            port = 1234;


        QString db_name;
        if(argc > 2)    db_name = QVariant(argv[2]).toString();
        else            db_name = "/home/letitbe/test_db";

        test_server * server = new test_server(port,db_name,"/home/letitbe/log_file.txt",&a);
        QObject::connect(server, &test_server::ServiceTerminated,&a,&QCoreApplication::quit,Qt::QueuedConnection);
        return a.exec();
    }

