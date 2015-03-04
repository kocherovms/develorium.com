#include <QByteArray>

int main() {
	QByteArray array;
	array.resize(1024);

	for(;;)
		array.resize(array.size() * 2);

	return 0;
}
