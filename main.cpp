#include "utils.h"
#include "quineMcCluskey.h"

int main()
{

    std::vector<std::string> s = { "A'B'C'D'","AB'C'D'","A'BC'D","A'BCD'","ABC'D'","A'BCD","AB'CD","ABCD'"};
    quineMcCluskey qm(&s);

    qm.start();

    return 0;
}