#ifndef _LEPSEL_H_
#define _LEPSEL_H_

#include <iostream>
#include "TString.h"

enum selection {DMu, EMu, DE, SE, SMu, err};

selection getLepSel(std::string);
TString getLepSel_string(selection);

#endif
