#include "wali/nwa/query/details/selectors.hpp"

#include <cassert>
#include <iostream>
#include <typeinfo>

using wali::Key;
using wali::nwa::NWA;
using namespace wali::nwa::query::details::selectors;

std::pair<Key, Key>
make_key_pair(int a, int b)
{
    return std::make_pair(Key(a), Key(b));
}

int main(int argc, char**argv)
{

    NWA::Call c(1, 2, 3);
    NWA::Internal i(10, 20, 30);
    NWA::Return r(100, 500, 200, 300);

    // Test the whole-transition selectors
    assert( CallTransitionSelector()(c) == c );
    assert( InternalTransitionSelector()(i) == i );
    assert( ReturnTransitionSelector()(r) == r );

    // Test the ground selectors
    assert( SourceSelector()(c) == 1 );
    assert( SourceSelector()(i) == 10 );
    assert( SourceSelector()(r) == 100 );

    assert( SymbolSelector()(c) == 2 );
    assert( SymbolSelector()(i) == 20 );
    assert( SymbolSelector()(r) == 200 );

    assert( TargetSelector()(c) == 3 );
    assert( TargetSelector()(i) == 30 );
    assert( TargetSelector()(r) == 300 );

    assert( CallPredecessorSelector()(r) == 500 );

    // Test the paired selector
    assert(( PairSelector<SourceSelector, TargetSelector>()(c) == make_key_pair(1, 3) ));
    assert(( PairSelector<SourceSelector, TargetSelector>()(i) == make_key_pair(10, 30) ));
    assert(( PairSelector<SourceSelector, TargetSelector>()(r) == make_key_pair(100, 300) ));

    assert(( PairSelector<SourceSelector, CallPredecessorSelector>()(r) == make_key_pair(100, 500) ));
    

    if (argc > 1) {
        std::cerr << "\n>> Tests good!\n";
        std::cerr << "\n****\nThere should be an error here; this is making sure assert is enabled\n";
        assert(false);
    }
}