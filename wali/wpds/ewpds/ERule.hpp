#ifndef wpds_ERULE_GUARD
#define wpds_ERULE_GUARD
/*
 * $Id: ERule.hpp,v 1.2 2005/06/17 17:42:30 akash Exp $
 * @author Akash Lal
 * @version $Revision: 1.2 $ 
 */

#include "../Rule.hpp"
#include "MergeFunction.hpp"
#include "../Config.hpp"

//
// begin namespace wali
//
namespace wali
{
  namespace wpds
  {
    namespace ewpds
    {
    
      /*! @class ERule
       *
       * @see Rule
       */

      class ERule : public Rule
      {

      public:
	friend class EWPDS;

      public: 

	/*!
	 * @brief constructor
	 */
	ERule( Config *f, Config *t, wali_key_t s2, sem_elem_t se, merge_fn_t fn = NULL );

	/*!
	 * @brief destructor
	 */
	~ERule();

	/*!
	 * @return Rule rhs second stack symbol or WALI_EPSILON if no such
	 * stack sym
	 */
	merge_fn_t merge_fn() const { return mf; }

	/*! @return const reference to this's Weight */
	const sem_elem_t extended_weight() const { return extended_se; }

	/*!
	 *
	 * @param o the std::ostream this is written to
	 * @return std::ostream passed in
	 *
	 * @see Printable
	 */
	std::ostream & print( std::ostream &o ) const;

	/*!
	 * marshall will write a ERule to a std::ostream. This writes
	 * the rule in XML form.
	 *
	 * @return std::ostream the marshalled rule was written to.
	 */
	std::ostream & marshall( std::ostream & o ) const;

      private:
	merge_fn_t mf;
	sem_elem_t extended_se;
      };
    } // end namespace ewpds
  } // end namespace wpds
} // end namespace wali

#endif  // wpds_ERULE_GUARD

/*
 * $Log: ERule.hpp,v $
 * Revision 1.2  2005/06/17 17:42:30  akash
 *
 * EWPDSDIR=WPDSDIR/ewpds
 *
 * Revision 1.1  2005/06/17 17:33:38  akash
 *
 * Change EWPDS directory to reflect change in namespace.
 *
 * Revision 1.2  2005/06/17 17:26:07  akash
 *
 * Move ewpds to its own namespace inside ::wali::wpds.
 *
 */

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
