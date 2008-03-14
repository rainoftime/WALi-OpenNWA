#ifndef wali_wfa_WFA_GUARD
#define wali_wfa_WFA_GUARD 1

/*!
 * @author Nick Kidd
 */

// ::wali
#include "wali/Common.hpp"
#include "wali/Printable.hpp"
#include "wali/SemElem.hpp"
#include "wali/HashMap.hpp"
#include "wali/KeyContainer.hpp"

// ::wali::wfa
#include "wali/wfa/WeightMaker.hpp"
#include "wali/wfa/Trans.hpp"
#include "wali/wfa/TransSet.hpp"

// std::c++
#include <iostream>
#include <list>
#include <set>

namespace wali
{
  template< typename T > class Worklist;

  namespace wpds
  {
    class WPDS;
    class DebugWPDS;
    class TransCopyLinker;
    namespace ewpds
    {
      class EWPDS;
    }
    namespace fwpds
    {
      class FWPDS;
    }
  }

  namespace regex {
    class Regex;
    typedef wali::ref_ptr<Regex> regex_t;
  }

  namespace wfa
  {
    class State;
    class TransFunctor;
    class ConstTransFunctor;

    /*! @class WFA
     *
     * TODO:
     *  - Should state_map_t have type HashMap< Key , ref_ptr<State> >
     *      This allows for automatic collection of States via HashMap
     *      State objects can escape the WFA (@see WPDS).
     */

    class WFA : public Printable
    {
      //
      // Types
      //
      public:
        /*!
         * @enum query_t
         * query_t determines how the weights are extended
         * during path_summary. INORDER is the normal each state's
         * weight is the combine of all outgoing transitions's weights
         * extended by the "to" state. REVERSE reverses this. INORDER
         * corresponds to a prestar query while REVERSE corresponds to
         * a poststar query when used with WPDS
         */
        enum query_t { INORDER,REVERSE,MAX };

        typedef wali::HashMap< KeyPair, TransSet > kp_map_t;
        typedef wali::HashMap< Key , State * > state_map_t;
        typedef wali::HashMap< Key , TransSet > eps_map_t;
        typedef std::set< State*,State > StateSet_t;
        typedef wali::HashMap< Key,StateSet_t > PredHash_t;

        friend class ::wali::wpds::WPDS;
        friend class ::wali::wpds::DebugWPDS;
        friend class ::wali::wpds::ewpds::EWPDS;
        friend class ::wali::wpds::fwpds::FWPDS;

        static const std::string XMLTag;
        static const std::string XMLQueryTag;
        static const std::string XMLInorderTag;
        static const std::string XMLReverseTag;

      protected:
      private:

        //
        // Methods
        //
      public:
        WFA( query_t q = INORDER );
        WFA( const WFA & rhs );
        WFA & operator=( const WFA & rhs );

        virtual ~WFA();

        /*!
         * Remove all transitions from the WFA
         */
        virtual void clear();

        /*!
         * @brief set initial state
         *
         * A WFA has 1 initial state. This method will
         * set it to the key parameter
         *
         * @param key the key for the desired initial state
         * @return Key that is the old initial stat
         */
        Key set_initial_state( Key key );

        /*!
         * @brief set initial state
         *
         * A WFA has 1 initial state. This method will
         * set it to the key parameter
         *
         * @param key the key for the desired initial state
         * @return Key that is the old initial stat
         */
        Key setInitialState( Key key );

        /*!
         * @brief Return the initial state
         *
         * @return Key for the initial state
         */
        Key initial_state() const;

        /*!
         * @brief Return the initial state
         *
         * @return Key for the initial state
         */
        Key getInitialState() const;

        /*!
         * Test if param key is the initial state.
         *
         * @return true if key == WFA::initial_state()
         */
        bool isInitialState( Key key ) const;

        /*!
         * Add parameter key to the set of final states
         */
        void add_final_state( Key key );

        /*!
         * Add parameter key to the set of final states
         */
        void addFinalState( Key key );

        /*!
         * Return true if parameter key is a final state
         */
        bool isFinalState( Key key ) const;

        /*!
         * Set the WFA query mode.
         * @return the old query mode
         */
        query_t setQuery( query_t newQuery );

        /*!
         * @return WFA query mode
         *
         * The query mode effects how weights are extended during
         * path summary. 
         *
         * @see WFA::query_t
         */
        query_t getQuery() const;

        /*!
         * @return The current generation
         *
         * Each WFA keeps a generation, which is
         * the number of times a reachability query has
         * been invoked on the WFA.
         */
        size_t getGeneration() const;

        /*!
         * @brief Set the generation to have the value of param g
         * @return void
         */
        void setGeneration(size_t g);

        /*! @brief Get a weight from the WFA. This is to get hold
         * of the weight domain class
         *
         * @see sem_elem_t
         * @return a weight on some transition in the WFA
         */
        virtual sem_elem_t getSomeWeight();

        /*! @brief Add transition (p,g,q) to the WFA
         *
         * @see Key
         * @see sem_elem_t
         */
        virtual void addTrans(
            Key p,
            Key g,
            Key q,
            sem_elem_t se );

        /*!
         * @brief Add Trans t to the WFA
         *
         * @see wali::wfa::Trans
         */
        virtual void addTrans( ITrans * t );

        /*!
         * @brief erase Trans
         *
         * Removes Trans (from,stack,to) from the WFA if it exists.
         */
        virtual void erase(
            Key from,
            Key stack,
            Key to );

        /*!
         * @brief erase State q and all of its outgoing transitions
         * It does not remove incoming transitions -- 
         * <b><i>that has to be done by the client</i></b>. 
         * 
         * This does not delete the wfa::State object. The destructor
         * will take care of reclaiming memory.
         *
         * @return true if it was sucessful
         */
        virtual bool eraseState( Key q );

        /*!
         * @brief return true if transition matching (p,g,q) exists
         * and copy that transition into the ref parameter t
         *
         * If a Trans matching (p,g,q) exists then it is copied into the
         * passed in reference "Trans & t". This way the user is not given
         * and handle to the WFA's internal Trans * pointer and deleting
         * all transitions associated with this WFA when it is deleted is
         * safe. However, because the weight of this transition is a
         * sem_elem_t (ref_ptr< SemElem >) modifying the weight from
         * the Trans & t will result in modification of the weight on
         * the interal Trans * object.
         *
         * @return bool true if transition matching (p,g,q) exists
         * @see Trans
         * @see SemElem
         * @see sem_elem_t
         */
        virtual bool find( 
            Key p,
            Key g,
            Key q,
            Trans & t );

        /*!
         * @brief invoke TransFunctor tf on each Trans
         */
        virtual void for_each( TransFunctor& tf );

        /*!
         * @brief invoke ConstTransFunctor on each Trans
         */
        virtual void for_each( ConstTransFunctor& tf ) const;

        /*!
         * Intersect this with parameter fa. This is a wrapper
         * for intersect( WeightMaker&,WFA& ) that passes
         * the WeightMaker KeepBoth.
         *
         * @see wali::wfa::WeightMaker
         * @see wali::wfa::KeepBoth
         */
        virtual WFA intersect( WFA& fa );

        /*!
         * Intersect this with parameter fa. This is a wrapper
         * for intersect( WeightMaker&,WFA&,WFA& ) that passes
         * the WeightMaker KeepBoth. Result is stored in dest.
         *
         * @see wali::wfa::WeightMaker
         * @see wali::wfa::KeepBoth
         */
        virtual void intersect( WFA& fa, WFA& dest );

        /*!
         * Intersect this with parameter fa and return the result
         * by value. The parameter WeightMaker determines how
         * intersection should join the weights on matching
         * transitions.
         *
         * @see wali::wfa::WeightMaker
         */
        WFA intersect( WeightMaker& wmaker , WFA& fa );

        /*!
         * Intersect this with parameter fa. The result is stored in
         * parameter dest. Parameter dest is cleared before any
         * results are stored. The parameter WeightMaker determines
         * how intersection should join the weights on matching
         * transitions.
         *
         * NOTE: For now this means (dest != this) && (dest != fa).
         */
        virtual void intersect( WeightMaker& wmaker , WFA& fa, WFA& dest );

        /*!
         * Performs path summary. Simply calls the path_summary with
         * the default Worklist provided by WALi.
         */
        virtual void path_summary();

        /*!
         * Performs path summary with the specified Worklist
         */
        virtual void path_summary( Worklist<State>& wl );

        /*!
         * Performs path summary. Simply calls the path_summary with
         * the default Worklist provided by WALi.
         * Initializes the weight on the final state to wt (can be
         * useful for efficiency in some cases)
         */
        virtual void path_summary(sem_elem_t wt);

        /*!
         * Prunes the WFA. This removes any transitions that are
         * not in the (getInitialState(),F) chop.
         */
        virtual void prune();

        /*!
         * Intersects the WFA with <init_state, (stk \Gamma^*)>
         * that essentially removes transitions and calls prune
         */
        virtual void filter(Key stk);

        /*!
         * Intersects the WFA with { <init_state, (stk \Gamma^*)> | stk
         * \in stkset }
         * This essentially removes transitions and calls prune
         */
        virtual void filter(std::set<Key> &stkset);

        /*!
         * Write the WFA to the std::ostream parameter, implements
         * Printable::print
         *
         * @param o the std::ostream this is written to
         * @return std::ostream WFA was written to
         *
         * @see Printable
         */
        virtual std::ostream & print( std::ostream & ) const;

        /*!
         * @brief Print WFA in dot format
         */
        virtual std::ostream& print_dot(
            std::ostream& o,
            bool print_weights=false ) const;

        /*!
         * @brief marshall WFA in XML 
         */
        virtual std::ostream& marshall( std::ostream& o ) const;

        virtual std::ostream& marshallState( std::ostream& o, Key key ) const;

        /*!
         * @brief inserts tnew into the WFA
         *
         * This method inserts Trans * tnew into the WFA.  If a transition
         * told matching (p,g,q) of tnew already exists, then tnew is
         * combined into told.
         *
         * @warning: Note that insert assumes ownership of the passed
         *      in parameter tnew
         * @warning: insert does assumes the states of tnew
         *      are already in the WFA. If they are not path_summary
         *      and other methods might fail
         *
         * @return pointer to real transition
         */
        ITrans * insert( ITrans * tnew );

        /*!
         * @brief Returns a TransSet containing all 
         * transitions matching (p,y,?) in the WFA
         *
         * @see TransSet
         */
        TransSet match( Key p, Key y);

        /*! 
         * Create a State * for the key Key
         */
        void addState( Key key , sem_elem_t zero );

        /*!
         * @return const pointer to underlying State object or NULL if
         * no such state exists.
         *
         * @note This may throw an exception in the future
         */
        const State* getState( Key name ) const;

        /*!
         * @return pointer to underlying State object or NULL if
         * no such state exists.
         *
         * @param name the key for this State
         * @note This may throw an exception in the future
         *
         */
        State * getState( Key name );

        const std::set< Key >& getStates() const;

        const std::set< Key >& getFinalStates() const;

        /* TODO
           size_t size() const;
           */

        size_t numStates() const
        {
          return Q.size();
        }

      protected:

        /*!
         * setupFixpoint clears each states markable flag and sets
         * the states weight to zero
         */
        void setupFixpoint( Worklist<State>& wl, PredHash_t& preds );

        /*!
         * setupFixpoint clears each states markable flag and sets
         * the states weight to zero, and weight of final states to
         * wtFinal (or ONE if NULL)
         */
        void setupFixpoint( Worklist<State>& wl, PredHash_t& preds, sem_elem_t wtFinal );

        /*!
         * Erases the specified Trans(from,stack,to) from the
         * kpmap and epsmap. A null return value means no transition existed.
         *
         * @return the Trans* that was removed from the maps
         */
        ITrans * eraseTransFromMaps(
            Key from,
            Key stack,
            Key to );
        /*!
         * Erases the specified Trans(from,stack,to) from the
         * kpmap. A null return value means no transition existed.
         *
         * @return the Trans* that was removed from the KpMap
         */
        ITrans* eraseTransFromKpMap(
            Key from,
            Key stack,
            Key to );

        ITrans* eraseTransFromKpMap( ITrans* terase );

        /*!
         * Erases the specified Trans(from,stack,to) from the
         * epsmap. A false return value means no transition existed.
         *
         * @return true if a transition was erased from EpsMap
         */
        ITrans* eraseTransFromEpsMap( ITrans* terase );

        /*!
         * Erases the State 'state' from the WFA and all transitions
         * that go to or from the State.
         *
         * @return true
         */
        bool eraseState( State* state );

        /*!
         * Performs path summary with the specified Worklist
         * Initializes the weight on the final state to wt (can be
         * useful for efficiency in some cases)
         */
        virtual void path_summary( Worklist<State>& wl, sem_elem_t wt );

      protected:
        regex::regex_t TarjanBasicRegex();

      private:
        // Poststar builds backwards WFAs so 
        // extend needs to take this into account.
        regex::regex_t EXTEND(regex::regex_t a,regex::regex_t b);

        //
        // Variables
        //
      public:
      protected:
        kp_map_t kpmap;          //! < map from KeyPair to list of trans
        state_map_t state_map;   //! < map from key to State
        eps_map_t eps_map;       //! < map from "to state" to list of eps trans ending in "to state"
        Key init_state;          //! < initial state of WFA
        std::set< Key > F;       //! < set of final states
        std::set< Key > Q;       //! < set of all states
        query_t query;           //! < determine the extend order for path_summary
        size_t generation;       //! < Each WPDS query increments the generation count.

      private:

    };

  } // namespace wfa

} // namespace wali

#endif  // wali_wfa_WFA_GUARD

/* Yo, Emacs!
   ;;; Local Variables: ***
   ;;; tab-width: 4 ***
   ;;; End: ***
   */
