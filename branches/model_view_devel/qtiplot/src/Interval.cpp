/***************************************************************************
    File                 : Interval.cpp
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
    Email (use @ for *)  : thzs*gmx.net
    Description          : Auxiliary class for interval based data
                           
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *  This program is free software; you can redistribute it and/or modify   *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation; either version 2 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  This program is distributed in the hope that it will be useful,        *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the Free Software           *
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    *
 *   Boston, MA  02110-1301  USA                                           *
 *                                                                         *
 ***************************************************************************/

#include "Interval.h"

void Interval::setStart(int start) 
{
	d_start = start;
	if(d_start > d_end && d_end != -1)
		qSwap(d_start, d_end);
}

void Interval::setEnd(int end) 
{
	d_end = end;
	if(d_start > d_end && d_start != -1)
		qSwap(d_start, d_end);
}

bool Interval::touches(const Interval& other) const 
{ 
	return ( (other.end() == d_start-1) || (other.start() == d_end+1) ); 
}

Interval Interval::merge(const Interval& a, const Interval& b)
{
	if( !(a.intersects(b) || a.touches(b)) )
		return a;
	return Interval( qMin(a.start(), b.start()), qMax(a.end(), b.end()) );
}

QList<Interval> Interval::subtract(const Interval& src_iv, const Interval& minus_iv)
{
	QList<Interval> list;
	if( (src_iv == minus_iv) || (minus_iv.contains(src_iv)) )
	{
		return list;
	}

	if( !src_iv.intersects(minus_iv) )
	{
		list.append(src_iv);
	}
	else if( src_iv.end() <= minus_iv.end() )
	{
		list.append( Interval(src_iv.start(), minus_iv.start()-1) );
	}
	else if( src_iv.start() >= minus_iv.start() )
	{
		list.append( Interval(minus_iv.end()+1, src_iv.end()) );
	}
	else
	{
		list.append( Interval(src_iv.start(), minus_iv.start()-1) );
		list.append( Interval(minus_iv.end()+1, src_iv.end()) );
	}

	return list;
}

void Interval::mergeIntervalIntoList(QList<Interval> * list, Interval i)
{
	for(int c=0; c<list->size(); c++)
	{
		if( list->at(c).touches(i) || list->at(c).intersects(i) )
		{
			Interval result = merge(list->takeAt(c), i);
			mergeIntervalIntoList(list, result);
			return;
		}
	}
	list->append(i);
}


void Interval::subtractIntervalFromList(QList<Interval> * list, Interval i)
{
		QList<Interval> temp_list;
		for(int c=0; c<list->size(); c++)
		{
			temp_list = subtract(list->at(c), i);
			if(temp_list.isEmpty())
				list->removeAt(c--);
			else 
			{
				list->replace(c, temp_list.at(0));
				if(temp_list.size()>1)
					list->insert(c, temp_list.at(1));
			}
		}
}
