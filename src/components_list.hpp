#pragma once

// these components should be defined in namespace "components".
// sep is needed to pass this list as template pack
#define COMPONENTS_LIST \
    XCOMPONENT(A, SEP)  \
    XCOMPONENT(B, SEP)  \
    XCOMPONENT(PositionComponent, )
