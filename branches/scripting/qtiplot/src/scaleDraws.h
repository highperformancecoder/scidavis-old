#ifndef SCALEDRAWS_H
#define SCALEDRAWS_H

#include "parser.h"

#include <qpainter.h>
#include <qdatetime.h>

#include <qwt_painter.h>
#include <qwt_scale_draw.h>

class ScaleDraw: public QwtScaleDraw
{
public:		
	ScaleDraw(const QString& s = QString::null):
	d_fmt('g'),
    d_prec(4),
	formula_string (s)
	{};

	virtual ~ScaleDraw(){};
		
	QString formulaString() {return formula_string;};
	void setFormulaString(const QString& formula) {formula_string = formula;};

	double transformValue(double value) const
	{
	if (!formula_string.isEmpty())
		{
		double lbl=0.0;
		try
			{
			myParser parser;
			if (formula_string.contains("x"))
				parser.DefineVar("x", &value);
			else if (formula_string.contains("y"))
				parser.DefineVar("y", &value);

			parser.SetExpr(formula_string.ascii());
			lbl=parser.Eval();
			}
		catch(mu::ParserError &)
			{
			return 0;
			}

		return lbl;
		}
	else
		return value;
	};

	virtual QwtText label(double value) const
	{
	return QwtText(QString::number(transformValue(value), d_fmt, d_prec));
	};

/*!
  \brief Set the number format for the major scale labels

  Format character and precision have the same meaning as for
  sprintf().
  \param f format character 'e', 'f', 'g' 
  \param prec
    - for 'e', 'f': the number of digits after the radix character (point)
    - for 'g': the maximum number of significant digits

  \sa labelFormat()
*/
void setLabelFormat(char f, int prec)
{
d_fmt = f;
d_prec = prec;
}

/*!
  \brief Set the number precision for the major scale labels

  Precision has the same meaning as for
  sprintf().
  \param prec
    - for 'e', 'f': the number of digits after the radix character (point)
    - for 'g': the maximum number of significant digits

  \sa labelFormat()
*/
void setLabelPrecision(int prec)
{
d_prec = prec;
}

/*!
  \brief Return the number format for the major scale labels

  Format character and precision have the same meaning as for
  sprintf().
  \param f format character 'e', 'f' or 'g' 
  \param prec
    - for 'e', 'f': the number of digits after the radix character (point)
    - for 'g': the maximum number of significant digits

  \sa setLabelFormat()
*/
void labelFormat(char &f, int &prec) const
{
    f = d_fmt;
    prec = d_prec;
}
	
	/*void drawTick(QPainter *p, double val, int len) const
	{
	bool print = p->device()->isExtDev();
		
	if (options() && !print)
		QwtScaleDraw::drawTick(p, val, len);
	else if (!options())
		{//axes without backbone
    	if ( len <= 0 )
       		return;

    	const int tval = transform(val);
		const int xorg = x();
		const int yorg = y();	
	
    	switch(orientation())
    		{
        	case Left:
				QwtPainter::drawLine(p, xorg + 1, tval, xorg - len, tval);
            break;

        	case Right:
				QwtPainter::drawLine(p, xorg, tval, xorg + len, tval);
            break;

        	case Bottom: 
                 QwtPainter::drawLine(p, tval, yorg, tval, yorg + len);
            break;

        	case Top:
				QwtPainter::drawLine(p, tval, yorg + 1, tval, yorg - len);
            break;

        	default:
            	break;
    		}
		}
	else if (print)
		{
    	if ( len <= 0 )
       		return;

    	const int tval = transform(val);
		const int xorg = x();
		const int yorg = y();	
	
    	switch(orientation())
    		{
        	case Left:
				{
				if (!options())
					QwtPainter::drawLine(p, xorg + 1, tval, xorg - len, tval);
				else
					QwtPainter::drawLine(p, xorg, tval, xorg - len, tval);
           	    break;
				}

        	case Right:
				{
				if (!options())
            		QwtPainter::drawLine(p, xorg, tval, xorg + len, tval);
				else
					{
					const int bw = QMAX (p->pen().width() / 2, 1);
					QwtPainter::drawLine(p, xorg + bw, tval, xorg + len, tval);
					}
            	break;
				}

        	case Bottom: 
				{
				const int bw = p->pen().width() / 2;
					QwtPainter::drawLine(p, tval, yorg + bw, tval, yorg + len);
            	break;
				}

        	case Top:
				QwtPainter::drawLine(p, tval, yorg, tval, yorg - len);
            break;

        	default:
            	break;
    		}
		}
	};*/
	
	/*void drawBackbone(QPainter *p) const
	{   
	const int pw = p->pen().width();
	const int pw2 = p->pen().width() % 2;
		
	if (pw ==1 || !pw2)
		{
		const int xorg = x();
		const int yorg = y();
		const int l = length();
			
    	switch(orientation())
    		{
        	case Left:
				{
				const int bw2 = (pw - 1) / 2;
            	QwtPainter::drawLine(p, xorg - bw2, yorg, xorg - bw2, yorg + l - 1);
            	break;
				}
			
        	case Right:
				{
				int bw2 = pw / 2;
				if (p->device()->isExtDev() && pw == 1)
					bw2 = 1;
					
				QwtPainter::drawLine(p, xorg + bw2, yorg, xorg + bw2, yorg + l - 1);
            	break;
				}
			
        	case Top:
				{
				const int bw2 = (pw - 1) / 2;
            	QwtPainter::drawLine(p, xorg, yorg - bw2, xorg + l - 1, yorg - bw2);
            	break;
				}
			
        	case Bottom:
				{
				const int bw2 = pw / 2;
            	QwtPainter::drawLine(p, xorg, yorg + bw2, xorg + l - 1, yorg + bw2);
            	break;
				}
			
       	 	case Round:
           	 break;
    		}
		}
	else
		QwtScaleDraw::drawBackbone(p);
	};*/
	
private:
	QString formula_string;

	char d_fmt;
    int d_prec;
};

class QwtTextScaleDraw: public ScaleDraw
{
public:
	QwtTextScaleDraw(const QStringList& list)
		{
		labels = list; 
		};
		
	~QwtTextScaleDraw(){};
		
	virtual QwtText label(double value) const
		{
		int index=qRound(value);
		if (floor(value) == value && index > 0 && index <= (int)labels.count())
			return QwtText(labels[index - 1]);
		else
			return QwtText();
		};

	QStringList labelsList(){return labels;};

private:
	QStringList labels;
};

class TimeScaleDraw: public ScaleDraw
{
public:
	TimeScaleDraw(const QTime& t, const QString& format)
		{
		t_origin = t; 
		t_format = format;
		};
		
	~TimeScaleDraw(){};
	
	QString origin() 
	{
		return t_origin.toString ( "hh:mm:ss.zzz" );
	};
	
	QString timeFormat() {return t_format;};
		
	virtual QwtText label(double value) const
	{
		QTime t = t_origin.addMSecs ( (int)value );		
		return QwtText(t.toString ( t_format ));
	};
	
private:
	QTime t_origin;
	QString t_format;
};

class DateScaleDraw: public ScaleDraw
{
public:
	DateScaleDraw(const QDate& t, const QString& format)
		{
		t_origin = t; 
		t_format = format;
		};
		
	~DateScaleDraw(){};
	
	QString origin() 
	{
		return t_origin.toString ( Qt::ISODate );
	};
	
	QString format() {return t_format;};
		
	virtual QwtText label(double value) const
	{
		QDate t = t_origin.addDays ( (int)value );
		return QwtText(t.toString ( t_format ));
	};
	
private:
	QDate t_origin;
	QString t_format;
};

class QwtSupersciptsScaleDraw: public ScaleDraw
{
public:
	QwtSupersciptsScaleDraw(const QString& s = QString::null)
	{
	setFormulaString(s);
	};

	~QwtSupersciptsScaleDraw(){};

	virtual QwtText label(double value) const
	{
	char f;
	int prec;
	labelFormat(f, prec);
	
	double val = transformValue(value);
	
	QString txt;
	txt.setNum (val, 'e', prec);

	QStringList list = QStringList::split ( "e", txt, FALSE );
	if (list[0].toDouble() == 0.0)
		return "0";
	
	QString s= list[1];
	int l = s.length();
	QChar sign = s[0];
	
	s.remove (sign);
		
	while (l>1 && s.startsWith ("0", false))
		{
		s.remove ( 0, 1 );
		l = s.length();
		}
		
	if (sign == '-')
		s.prepend(sign);
	
	if (list[0] == "1")
		return QwtText("10<sup>" + s + "</sup>");
	else
		return QwtText(list[0] + "x10<sup>" + s + "</sup>");
	};
};

#endif
