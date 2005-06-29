/*!
 * @author Nick Kidd
 * @version $Revision: 1.5 $
 */

#include "wali/WitnessCombine.hpp"
#include <algorithm>

namespace wali
{
    //
    // Pass weight to base class Witness
    //
    WitnessCombine::WitnessCombine( sem_elem_t weight ) :
        Witness( weight ) {}

    //
    // Destructor does nothing.
    //
    WitnessCombine::~WitnessCombine() {}

    //
    // Override Witness::combine
    //
    sem_elem_t WitnessCombine::combine( SemElem * se )
    {
        // TODO : make this a debug check
        // and non debug use static_cast
        Witness *that = dynamic_cast< Witness * >(se);
        if( 0 == that )
        {
            // error checking
            std::cerr << "SemElem is \"" << typeid(se).name() << "\"\n";
            assert( 0 );
        }

        // Perform the combine here b/c both branches need the combined
        // weight.
        sem_elem_t combined_se = user_se->combine(that->weight());

        // Always create a new WitnessCombine b/c cycles cause mucho problemo
        // with ref counting.
        WitnessCombine * newwc = new WitnessCombine( combined_se );
        WitnessCombine * oldwc = dynamic_cast< WitnessCombine* >(that);

        //      if that is a WitnessCombine then we might just incorporate
        //      that's children into us. O/w incorporate us into that.
        if( 0 != oldwc )
        {
            // SemElem param (that) is already a WitnessCombine.
            // We have two WitnessCombines. Determine which children newwc
            // will get

            // if A + B == A, then A >= B in the semiring.
            // Use above to see if oldwc->user_se contains
            // this->user_se.
            //
            if( combined_se->equal( user_se ) ) {
                // this->user_se contains oldwc->user_se
                newwc->absorb(this);
            }
            else if( combined_se->equal( oldwc->user_se ) )
            {
                // oldwc->user_se contains this->user_se
                newwc->absorb(oldwc);
            }
            else {
                // neither user_se contained the other
                newwc->absorb(this);
                newwc->absorb(oldwc);
            }

        }
        else {
            // that was not a WitnessCombine.
            // Check to see if we already contain that->user_se
            if( !user_se->equal(combined_se) )
            {
                // no containment.
                // Copy our children into newwc
                newwc->absorb(this);
            }
            // Add "that" to wc
            newwc->add_child(that);
        }
        return newwc;
    }

    //
    // Override Witness::pretty_print
    //
    std::ostream& WitnessCombine::pretty_print( std::ostream& o, size_t depth ) const
    {
        format_depth(o,depth);
        o << "WitnessCombine: ";
        user_se->print(o) << std::endl;
        std::list< witness_t >::const_iterator it = kids.begin();
        std::list< witness_t >::const_iterator itEND = kids.end();
        for( ; it != itEND ; it++ )
        {
            (*it)->pretty_print(o,depth+1);
        }
        return o;

    }

    //
    // Add a child to this
    //
    void WitnessCombine::add_child( witness_t w )
    {
        std::cerr << "[WitnessCombine::add_child]\n";
        kids.push_back( w );
    }

    //
    // The combine of all the weights of the children should (must)
    // equal the weight of this WitnessCombine object.
    //
    // @return reference to list of Witness children.
    //
    std::list< witness_t >& WitnessCombine::children()
    {
        return kids;
    }

    void WitnessCombine::absorb( WitnessCombine * wc )
    {
        std::copy(wc->children().begin(),wc->children().end(),std::back_inserter(kids));
    }


}

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
