/*!
 * @author Nick Kidd
 */

#include <sstream>
#include <cassert>
#include "wali/KeyFactory.hpp"
#include "wali/wpds/Rule.hpp"
#include "wali/wpds/Config.hpp"

namespace wali
{
    namespace wpds
    {

        int Rule::numRules = 0;

        Rule::Rule( Config *f_, Config *t_, wali_key_t stk2_, sem_elem_t se_ ) :
            f(f_),t(t_),stk2(stk2_),se(se_)
        {
            numRules++;
            std::cerr << "Rule(...) : " << numRules << std::endl;
        }

        Rule::~Rule()
        {
            numRules--;
            std::cerr << "~Rule()   : " << numRules << std::endl;
        }

        const Config & Rule::from() const { return *f; }

        /*!
         * @return from state key
         */
        wali_key_t Rule::from_state() const { return from().state(); }

        /*!
         * @return from stack key
         */
        wali_key_t Rule::from_stack() const { return from().stack(); }

        /*!
         * @return Config Rule transitions to
         */
        const Config & Rule::to() const { return *t; }

        /*!
         * @return to state key
         */
        wali_key_t Rule::to_state() const { return to().state(); }

        /*!
         * @return to stack key 1
         */
        wali_key_t Rule::to_stack1() const { return to().stack(); }

        /*!
         * @return to stack key 2
         */
        wali_key_t Rule::to_stack2() const { return stack2(); }

        /*!
         * @return const reference to this's Weight
         */
        const sem_elem_t& Rule::weight() const { return se; }

        /*! @return the Rule's weight */
        sem_elem_t Rule::weight() { return se; }
        
        /*! sets the weight of the Rule */
        void Rule::weight( sem_elem_t wnew )
        {
            se = wnew;
        }

        std::ostream & Rule::print( std::ostream &o ) const
        {
            o << "<";
            KeyFactory::key2str(from_state());
            o << ", ";
            KeyFactory::key2str(from_stack());
            o << "> -> <";
            KeyFactory::key2str(to_state());
            o << ", ";
            wali_key_t stk1 = to_stack1();
            if( stk1 != WALI_EPSILON )
            {
                KeyFactory::key2str(stk1);
                if( stk2 != WALI_EPSILON )
                {
                    o << " ";
                    KeyFactory::key2str(to_stack2());
                }
            }
            else {
                // sanity check
                assert( WALI_EPSILON == to_stack2());
            }
            o << ">";
            o << "\t" << weight()->to_string();
            return o;
        }

        std::ostream & Rule::marshall( std::ostream & o ) const
        {
            o << "<rule>\n";
            o << "\t<fromstate>" << KeyFactory::key2str(from_state()) << "</fromstate>\n";
            o << "\t<fromstack>" << KeyFactory::key2str(from_stack()) << "</fromstack>\n";
            o << "\t<tostate>" << KeyFactory::key2str(to_state()) << "</tostate>\n";

            // Check optional stack symbols
            if( WALI_EPSILON != to_stack1() ) {
                o << "\t<tostack1>" << KeyFactory::key2str(to_stack1()) << "</tostack1>\n";
                if( WALI_EPSILON != to_stack2() ) {
                    o << "\t<tostack2>" << KeyFactory::key2str(to_stack2()) << "</tostack2>\n";
                }
            }
            else {
                // sanity check
                assert( WALI_EPSILON == to_stack2() );
            }
            o << "\t<weight>" << weight()->to_string() << "</weight>\n";
            o << "</rule>";
            return o;
        }

    } // namespace wpds

} // namespace wali

/* Yo, Emacs!
;;; Local Variables: ***
;;; tab-width: 4 ***
;;; End: ***
*/
