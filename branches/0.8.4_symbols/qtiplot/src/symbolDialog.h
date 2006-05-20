#ifndef SYMBOLDIALOG_H
#define SYMBOLDIALOG_H

#include <qvariant.h>
#include <qdialog.h>

class QButtonGroup;
class QPushButton;

class symbolDialog : public QDialog
{
    Q_OBJECT

public:
	//! Character set
	enum CharSet{
		lowerGreek = 0, /*!< lower case Greek letters */
		upperGreek = 1,  /*!< upper case Greek letters */
		mathSymbols = 2, /*!< mathematical symbols */
		arrowSymbols = 3, /*!< arrow symbols */
	};

    symbolDialog(CharSet charsSet, QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~symbolDialog();

	//! Show lowercase Greek characters
	void initLowerGreekChars();
	//! Show uppercase Greek characters
	void initUpperGreekChars();
	//! Show mathematical symbols
	void initMathSymbols();
	//! Show arrow symbols
	void initArrowSymbols();

    QButtonGroup *GroupBox1;

public slots:
    virtual void languageChange();
	void getChar(int btnIndex);
	void addCurrentChar();

signals:
	void addLetter(const QString&);
};

#endif // exportDialog_H
