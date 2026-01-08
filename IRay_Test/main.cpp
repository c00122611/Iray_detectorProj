#include <QtWidgets/QApplication>
#include "IrayWidget.h"
int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	IrayWidget window;
	window.show();
	return app.exec();
}