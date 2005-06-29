#ifndef wali_WITNESS_COMBINE_GUARD
#define wali_WITNESS_COMBINE_GUARD 1

/*!
 * @author Nick Kidd
 * @version $Revision: 1.5 $
 */

#include "wali/Common.hpp"
#include "wali/Witness.hpp"
#include <list>

namespace wali
{
    class WitnessCombine : public Witness
    {
        public:
            /*!
             * Pass weight to base class Witness
             */
            WitnessCombine( sem_elem_t weight );

            /*!
             * Destructor does nothing.
             */
            virtual ~WitnessCombine();

            /*!
             * @brief Override Witness::combine
             *
             * Overriding Witness::combine here allows for Witness::combine
             * to not have to check if "this" is actually of type
             * WitnessCombine b/c if it were they dynamic dispatch would have
             * ended up here.
             */
            virtual sem_elem_t combine( SemElem * se );

            /*!
             * Override Witness::pretty_print
             */
            virtual std::ostream& pretty_print( std::ostream& o, size_t depth ) const;

            /*!
             * Add a child to this
             */
            void add_child( witness_t w );

            /*!
             * The combine of all the weights of the children should (must)
             * equal the weight of this WitnessCombine object.
             *
             * @return reference to list of Witness children.
             */
            std::list< witness_t >& children();

            /*!
             * absorb param wc's children into this
             */
            void absorb( WitnessCombine * wc );

        protected:
            std::list< witness_t > kids;

    }; // class WitnessCombine

} // namespace wali

#endif  // wali_WITNESS_COMBINE_GUARD

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/

