#include <QMessageBox>

#define CUS_YES_OR_NO(string) \
	(QMessageBox::question( \
	this, \
	tr("提示"), \
	tr(string), \
	QMessageBox::Yes | QMessageBox::No, \
	QMessageBox::Yes \
	) == QMessageBox::Yes)

#define CUS_YES_NO_OR_CANCEL(string) \
	QMessageBox::question( \
	this, \
	tr("提示"), \
	tr(string), \
	QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, \
	QMessageBox::Yes \
	)