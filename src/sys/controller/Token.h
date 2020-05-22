/* This file is part of COVISE.

   You can use it under the terms of the GNU Lesser General Public License
   version 2.1 or later, see lgpl-2.1.txt.

 * License: LGPL 2+ */

#ifndef TOKEN_INCLUDED
#define TOKEN_INCLUDED

namespace covise
{

class Token
{
private:
    char *string;
    char *position;

public:
    Token(const char *s);
    char *next(); // returns next token, \n separated
    char *nextSpace(); // returns next token, \n or space separated
    ~Token();
};
}
#endif
