/***************************************************************************
    File                 : Interval.h
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

// TODO: remove this after testing
#include <QtDebug>

//! Auxiliary class for interval based data
/**
 *	This class represents an interval of
 *	the type [start,end]. It should be pretty 
 *	self explanatory.
 */
class Interval
{
public:
	Interval() : d_start(-1), d_end(-1){}
	Interval(int start, int end) : d_start(start), d_end(end){}
	int start() const { return d_start; }
	int end() const { return d_end; }
	int size() const { return d_end - d_start + 1; }
	void setStart(int start);
	void setEnd(int end);
	bool isValid() const { return ( d_start >= 0 && d_end >= 0 && d_start <= d_end ); }
	bool contains(const Interval& other) const { return ( d_start <= other.start() && d_end >= other.end() ); }
	bool contains(int value) const { return ( d_start <= value && d_end >= value ); }
	bool intersects(const Interval& other) const { return ( contains(other.start()) || contains(other.end()) ); }
	bool touches(const Interval& other) const;
	void translate(int offset) { d_start += offset; d_end += offset; }
	bool operator==(Interval other) const { return ( d_start == other.start() && d_end == other.end() ); }
	//! Merge two intervals that touch or intersect
	static Interval merge(const Interval& a, const Interval& b);
	//! Subtract an interval from another
	static QList<Interval> subtract(const Interval& src_iv, const Interval& minus_iv);
	//! Merge an interval into a list
	/*
	 * This function merges all intervals in the list until none of them
	 * intersect of touch anymore.
	 */
	static void mergeIntervalIntoList(QList<Interval> * list, Interval i);
	//! Substact an interval from all intervals in the list
	/**
	 * Remark: This may increase or decrease the list size.
	 */
	static void subtractIntervalFromList(QList<Interval> * list, Interval i);

// TODO: remove this after testing
	void print() const
	{
		qDebug() << "[" << d_start << "," << d_end << "]";
	}
private:
	//! Interval start
	int d_start; 
	//! Interval end
	int d_end;
};

