#include "flb_avoperator.h"
#include <QtWidgets/QApplication>
#include <QDebug>


#include "avutils.h"



int main(int argc, char* argv[])
{
	//for (int s = 0; s < 300; ++s)
	//{
	//	Sleep(100);
	//	QList<FrameSPtr> arr;
	//	QList<int> ia;
	//	for (int i = 0; i < 1000000; ++i)
	//	{
	//		/*auto f = std::make_shared<FFrame>();
	//		auto pts = f->getPts();
	//		arr.append(f);*/
	//		//arr.append(nullptr);
	//		ia.append(i);
	//	}
	//	//qDebug() << arr.back()->getPts();
	//	qDebug() << ia[max(rand() % ia.size(), 0)];
	//	arr.clear();
	//	ia.clear();
	//	qDebug() << s << getCurrentMemoryUsage();
	//	Sleep(100);
	//}

	QApplication a(argc, argv);
	FLB_AVOperator w;
	w.show();
	return a.exec();
}
