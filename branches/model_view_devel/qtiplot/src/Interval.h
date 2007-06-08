/***************************************************************************
    File                 : Interval.h
    Project              : QtiPlot
    --------------------------------------------------------------------
    Copyright            : (C) 2007 by Tilman Hoener zu Siederdissen,
                           Knut Franke
    Email (use @ for *)  : thzs*gmx.net, knut.franke*gmx.de
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

#ifndef INTERVAL_H
#define INTERVAL_H

#include <QList>
#include <QString>

// forward declaration
template<class T> class Interval;

template<class T> class IntervalBase
{
	public:
		IntervalBase() : d_start(-1), d_end(-1){}
		IntervalBase(const IntervalBase<T>& other) {
			d_start = other.start();
			d_end = other.end();
		}
		IntervalBase(T start, T end) {
			d_start = start;
			d_end = end;
		}
		virtual ~IntervalBase() {}
		T start() const { return d_start; }
		T end() const { return d_end; }
		void setStart(T start) { d_start = start; }
		void setEnd(T end) { d_end = end; }
		bool contains(const Interval<T>& other) const { return ( d_start <= other.start() && d_end >= other.end() ); }
		bool contains(T value) const { return ( d_start <= value && d_end >= value ); }
		bool intersects(const Interval<T>& other) const { return ( contains(other.start()) || contains(other.end()) ); }
		void translate(T offset) { d_start += offset; d_end += offset; }
		bool operator==(const Interval<T>& other) const { return ( d_start == other.start() && d_end == other.end() ); }
		Interval<T>& operator=(const Interval<T>& other) {
			d_start = other.start();
			d_end = other.end();
			return *this;
		}
		//! Returns true if no gap is between two intervals.
		virtual bool touches(const Interval<T>& other) const = 0;
		//! Merge two intervals that touch or intersect
		static Interval<T> merge(const Interval<T>& a, const Interval<T>& b) {
			if( !(a.intersects(b) || a.touches(b)) )
				return a;
			return Interval<T>( qMin(a.start(), b.start()), qMax(a.end(), b.end()) );
		}
		//! Subtract an interval from another
		static QList<Interval<T> > subtract(const Interval<T>& src_iv, const Interval<T>& minus_iv) {
			QList<Interval<T> > list;
			if( (src_iv == minus_iv) || (minus_iv.contains(src_iv)) )
				return list;

			if( !src_iv.intersects(minus_iv) )
				list.append(src_iv);
			else if( src_iv.end() <= minus_iv.end() )
				list.append( Interval<T>(src_iv.start(), minus_iv.start()-1) );
			else if( src_iv.start() >= minus_iv.start() )
				list.append( Interval<T>(minus_iv.end()+1, src_iv.end()) );
			else {
				list.append( Interval<T>(src_iv.start(), minus_iv.start()-1) );
				list.append( Interval<T>(minus_iv.end()+1, src_iv.end()) );
			}

			return list;
		}
		//! Split an interval into two
		static QList<Interval<T> > split(const Interval<T>& i, T before) {
			QList<Interval<T> > list;
			if( before < i.start() || before > i.end() )
			{
				list.append(i);
			}
			else
			{
				Interval<T> left(i.start(), before-1);
				Interval<T> right(before, i.end());
				if(left.isValid())
					list.append(left);
				if(right.isValid())
					list.append(right);
			}
			return list;
		}
		//! Merge an interval into a list
		/*
		 * This function merges all intervals in the list until none of them
		 * intersect of touch anymore.
		 */
		static void mergeIntervalIntoList(QList<Interval<T> > * list, Interval<T> i) {
			for(int c=0; c<list->size(); c++)
			{
				if( list->at(c).touches(i) || list->at(c).intersects(i) )
				{
					Interval<T> result = merge(list->takeAt(c), i);
					mergeIntervalIntoList(list, result);
					return;
				}
			}
			list->append(i);
		}
		//! Substact an interval from all intervals in the list
		/**
		 * Remark: This may increase or decrease the list size.
		 */
		static void subtractIntervalFromList(QList<Interval<T> > * list, Interval<T> i) {
			QList<Interval<T> > temp_list;
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
	//! Return a string in the format '[start,end]'
	QString toString() const {
		return "[" + QString::number(d_start) + "," + QString::number(d_end) + "]";
	}

	protected:
		//! Interval start
		T d_start;
		//! Interval end
		T d_end;
};

//! Auxiliary class for interval based data
/**
 *	This class represents an interval of
 *	the type [start,end]. It should be pretty
 *	self explanatory.
 *
 *	For the template argument (T), only numerical types ((unsigned) short, (unsigned) int,
 *	(unsigned) long, float, double, long double) are supported.
 */
template<class T=int> class Interval : public IntervalBase<T>
{
	public:
		Interval() {}
		Interval(T start, T end) : IntervalBase<T>(start, end) {}
		Interval(const Interval<T>& other) : IntervalBase<T>(other) {}
		T size() const { return IntervalBase<T>::d_end - IntervalBase<T>::d_start; }
		bool isValid() const { return ( IntervalBase<T>::d_start <= IntervalBase<T>::d_end ); }
		bool touches(const Interval<T>& other) const {
			return ( (other.end() == IntervalBase<T>::d_start) ||
					(other.start() == IntervalBase<T>::d_end) );
		}
};

template<> class Interval<short> : public IntervalBase<short>
{
	public:
		Interval() {}
		Interval(short start, short end) : IntervalBase<short>(start, end) {}
		Interval(const Interval<short>& other) : IntervalBase<short>(other) {}
		short size() const {
			return IntervalBase<short>::d_end - IntervalBase<short>::d_start + 1;
		}
		bool isValid() const {
			return ( IntervalBase<short>::d_start >= 0 && IntervalBase<short>::d_end >= 0 &&
					IntervalBase<short>::d_start <= IntervalBase<short>::d_end );
		}
		bool touches(const Interval<short>& other) const {
			return ( (other.end() == IntervalBase<short>::d_start-1) ||
					(other.start() == IntervalBase<short>::d_end+1) );
		}
};

template<> class Interval<unsigned short> : public IntervalBase<unsigned short>
{
	public:
		Interval() {}
		Interval(unsigned short start, unsigned short end) : IntervalBase<unsigned short>(start, end) {}
		Interval(const Interval<unsigned short>& other) : IntervalBase<unsigned short>(other) {}
		unsigned short size() const {
			return IntervalBase<unsigned short>::d_end - IntervalBase<unsigned short>::d_start + 1;
		}
		bool isValid() const {
			return ( IntervalBase<unsigned short>::d_start <= IntervalBase<unsigned short>::d_end );
		}
		bool touches(const Interval<unsigned short>& other) const {
			return ( (other.end() == IntervalBase<unsigned short>::d_start-1) ||
					(other.start() == IntervalBase<unsigned short>::d_end+1) );
		}
};

template<> class Interval<int> : public IntervalBase<int>
{
	public:
		Interval() {}
		Interval(int start, int end) : IntervalBase<int>(start, end) {}
		Interval(const Interval<int>& other) : IntervalBase<int>(other) {}
		int size() const { return IntervalBase<int>::d_end - IntervalBase<int>::d_start + 1; }
		bool isValid() const {
			return ( IntervalBase<int>::d_start >= 0 && IntervalBase<int>::d_end >= 0 &&
					IntervalBase<int>::d_start <= IntervalBase<int>::d_end );
		}
		bool touches(const Interval<int>& other) const {
			return ( (other.end() == IntervalBase<int>::d_start-1) ||
					(other.start() == IntervalBase<int>::d_end+1) );
		}
};

template<> class Interval<unsigned int> : public IntervalBase<unsigned int>
{
	public:
		Interval() {}
		Interval(unsigned int start, unsigned int end) : IntervalBase<unsigned int>(start, end) {}
		Interval(const Interval<unsigned int>& other) : IntervalBase<unsigned int>(other) {}
		unsigned int size() const {
			return IntervalBase<unsigned int>::d_end - IntervalBase<unsigned int>::d_start + 1;
		}
		bool isValid() const {
			return ( IntervalBase<unsigned int>::d_start <= IntervalBase<unsigned int>::d_end );
		}
		bool touches(const Interval<unsigned int>& other) const {
			return ( (other.end() == IntervalBase<unsigned int>::d_start-1) || (other.start() == IntervalBase<unsigned int>::d_end+1) );
		}
};

template<> class Interval<long> : public IntervalBase<long>
{
	public:
		Interval() {}
		Interval(long start, long end) : IntervalBase<long>(start, end) {}
		Interval(const Interval<long>& other) : IntervalBase<long>(other) {}
		long size() const {
			return IntervalBase<long>::d_end - IntervalBase<long>::d_start + 1;
		}
		bool isValid() const {
			return ( IntervalBase<long>::d_start >= 0 && IntervalBase<long>::d_end >= 0 &&
					IntervalBase<long>::d_start <= IntervalBase<long>::d_end );
		}
		bool touches(const Interval<long>& other) const {
			return ( (other.end() == IntervalBase<long>::d_start-1) ||
					(other.start() == IntervalBase<long>::d_end+1) );
		}
};

template<> class Interval<unsigned long> : public IntervalBase<unsigned long>
{
	public:
		Interval() {}
		Interval(unsigned long start, unsigned long end) : IntervalBase<unsigned long>(start, end) {}
		Interval(const Interval<unsigned long>& other) : IntervalBase<unsigned long>(other) {}
		unsigned long size() const {
			return IntervalBase<unsigned long>::d_end - IntervalBase<unsigned long>::d_start + 1;
		}
		bool isValid() const {
			return ( IntervalBase<unsigned long>::d_start <= IntervalBase<unsigned long>::d_end );
		}
		bool touches(const Interval<unsigned long>& other) const {
			return ( (other.end() == IntervalBase<unsigned long>::d_start-1) ||
					(other.start() == IntervalBase<unsigned long>::d_end+1) );
		}
};

#endif

