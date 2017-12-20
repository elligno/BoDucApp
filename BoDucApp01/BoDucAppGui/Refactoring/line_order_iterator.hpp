#pragma once

// C++ include
#include <iterator>

namespace bdGui 
{
	// Iterator to iterator through line (purchase order)
	// for the BoDuc application
	// Usage
	//  
	template<typename Iter> // std::vector<std::string>::const_iterator for BoDucApp
	class bdline_order_iterator : public std::iterator<       // inherit from std::iterator
		typename std::iterator_traits<Iter>::iterator_category, // iterator category
		typename std::iterator_traits<Iter>::value_type,        // iterator value
		typename std::iterator_traits<Iter>::difference_type,   // iterator difference
		const typename std::iterator_traits<Iter>::pointer,     // iterator pointer
		const typename std::iterator_traits<Iter>::reference    // iterator reference
	> 
	{
	public:
		/*  These typedefs are needed even though inherited from
		std::iterator (see 14.6.2.3 of the standard).
		*/
		typedef typename std::iterator_traits<Iter>::value_type value_type;
		typedef typename std::iterator_traits<Iter>::difference_type
			difference_type;
		typedef const value_type* pointer;
		typedef const value_type& reference;

		// ctor 
		bdline_order_iterator(const Iter& aIter);
		// do we need the other ctor or default ones are ok?

		// shall be in const context? usually it is.  
		pointer   operator-> () const;
		reference operator*  () const;
		// i am not sure i understand this operator implementation?
		value_type operator[] (int32 aI) const;

		//
		// iterator arithmetic (advance the underlying iterator)
		// 

		// pre and post incremental operator
		bdline_order_iterator& operator++ ();      // increment, return the modified iterator
		bdline_order_iterator  operator++ (int32); // increment, return the original iterator 
		bdline_order_iterator& operator-- ();      // increment, return the modified iterator
		bdline_order_iterator  operator-- (int32); // increment, return the original iterator
		bdline_order_iterator  operator+  (difference_type aDiff); // what we do exactly?
		bdline_order_iterator  operator-  (difference_type aDiff);
		bdline_order_iterator& operator+= (difference_type aDiff);
		bdline_order_iterator& operator-= (difference_type aDiff);

		difference_type operator- (const bdline_order_iterator<Iter>& aOther);
	private:
		Iter m_iter;                  // underlying iterator
		mutable value_type m_valType; // type that iterator pointed to
	};

	// utility to create an iterator on-the-fly
	template<class Iter>
	bdline_order_iterator<Iter>
		make_bdline_order_iterator(Iter aIter);

	//
	// Inline definition
	//

	// ____________________________________________________
	//
	// ctor
	template<class Iter, class Func>
	bdline_order_iterator<Iter>::bdline_order_iterator(const Iter& aIter)
	: m_iter(aIter)
	{
		// do nothing
	}

} // End of namespace
