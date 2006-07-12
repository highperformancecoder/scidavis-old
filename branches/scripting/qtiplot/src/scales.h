#ifndef SCALEDRAWS_H
#define SCALEDRAWS_H

#include <qwt_scale_draw.h>

#include <qstringlist.h>
#include <qdatetime.h>

class ScaleDraw: public QwtScaleDraw
{
public:		
	ScaleDraw(const QString& s = QString::null);
	virtual ~ScaleDraw(){};
		
	QString formulaString() {return formula_string;};
	void setFormulaString(const QString& formula) {formula_string = formula;};

	double transformValue(double value) const;

	virtual QwtText label(double value) const
	{
	return QwtText(QString::number(transformValue(value), d_fmt, d_prec));
	};


	void setLabelFormat(char f, int prec);
	void setLabelPrecision(int prec);

	void labelFormat(char &f, int &prec) const;

	//virtual void drawTick(QPainter *p, double val, int len) const;
	virtual void drawBackbone(QPainter *p) const;
	
private:
	QString formula_string;
	char d_fmt;
    int d_prec;
};

class QwtTextScaleDraw: public ScaleDraw
{
public:
	QwtTextScaleDraw(const QStringList& list);
	~QwtTextScaleDraw(){};
		
	virtual QwtText label(double value) const;

	QStringList labelsList(){return labels;};
private:
	QStringList labels;
};

class TimeScaleDraw: public ScaleDraw
{
public:
	TimeScaleDraw(const QTime& t, const QString& format);	
	~TimeScaleDraw(){};
	
	QString origin();
	QString timeFormat() {return t_format;};
		
	virtual QwtText label(double value) const;
	
private:
	QTime t_origin;
	QString t_format;
};

class DateScaleDraw: public ScaleDraw
{
public:
	DateScaleDraw(const QDate& t, const QString& format);
	~DateScaleDraw(){};
	
	QString origin();
	
	QString format() {return t_format;};
	virtual QwtText label(double value) const;
	
private:
	QDate t_origin;
	QString t_format;
};

class WeekDayScaleDraw: public ScaleDraw
{
public:
	enum NameFormat{ShortName, LongName, Initial};

	WeekDayScaleDraw(NameFormat format = ShortName);	
	~WeekDayScaleDraw(){};
		
	NameFormat format() {return d_format;};
	virtual QwtText label(double value) const;
	
private:
	NameFormat d_format;
};

class MonthScaleDraw: public ScaleDraw
{
public:
	enum NameFormat{ShortName, LongName, Initial};

	MonthScaleDraw(NameFormat format = ShortName);	
	~MonthScaleDraw(){};
		
	NameFormat format() {return d_format;};	
	virtual QwtText label(double value) const;
	
private:
	NameFormat d_format;
};

class QwtSupersciptsScaleDraw: public ScaleDraw
{
public:
	QwtSupersciptsScaleDraw(const QString& s = QString::null);
	~QwtSupersciptsScaleDraw(){};

	virtual QwtText label(double value) const;
};

#endif
