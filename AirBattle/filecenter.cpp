#include "filecenter.h"
#include "QDirIterator"

FileCenter::FileCenter()
{
}


QStringList FileCenter::AI_List()
{
    QDirIterator a("AI");

    QStringList ans;
    while (a.hasNext())
    {
        QString tmp = a.next();
        tmp = tmp.mid(3, tmp.size() - 3);
        if (tmp == ".." || tmp == ".") continue;
        bool tag = false;
        for (int i = 0; i < tmp.size(); i++)
            if (tmp.mid(i, 1) == ".")
                tag = true;
        if (tag) continue;
        ans.push_back(tmp);
    }

    return ans;
}

QStringList FileCenter::Record_List()
{
    QDirIterator a("Record");

    QStringList ans;
    while (a.hasNext())
    {
        QString tmp = a.next();
        tmp = tmp.mid(7, tmp.size() - 3);
        if (tmp == ".." || tmp == ".") continue;
        if (tmp.size() <= 4) continue;
        if (tmp.mid(tmp.size()-4, 4) != ".txt") continue;
        tmp = tmp.mid(0, tmp.size()-4);
        ans.push_back(tmp);
    }
    return ans;
}
