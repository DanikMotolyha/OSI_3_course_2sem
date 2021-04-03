// lab02.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#define FILE_NAME "file.txt"
#define DEFAULT_FILE_NAME "default.ht"
using namespace std;
#include "../StaticLibHTSTORAGE/HT.h"
int main()
{
    HT::HTHANDLE* ht = nullptr;
    HT::HTHANDLE* ht22 = nullptr;

    try {
        ht = HT::Create(1000, 2, 10, 256, FILE_NAME);
        if (ht)cout << "create succes\n";
        else throw "create error";

        if (HT::Snap(ht))cout << "Good snap\n";
        else cout << "bad snap";

        if (HT::Insert(ht, new HT::Element("key2222", 7, "payload", 7)))
            cout << "Insert good\n";
        else cout << "Insert error\n";

        HT::Element* hte = HT::Get(ht, new HT::Element("key2222", 7));
        if (hte)cout << "Get\n";
        else throw "Get error";

        HT::print(hte);

        if (HT::Update(ht, hte, "newload2", 8))cout << "Update good\n";
        else cout << "update bad";

        HT::print(hte);

        if (HT::Insert(ht, new HT::Element("key2222", 7, "payload", 7)))
            cout << "Insert good\n";
        else cout << "Insert error\n";
        if (HT::Insert(ht, new HT::Element("key4444", 7, "payload", 7)))
            cout << "Insert good\n";
        else cout << "Insert error\n";
        if (HT::Insert(ht, new HT::Element("key5555", 7, "payload", 7)))
            cout << "Insert good\n";
        else cout << "Insert error\n";
        if (HT::Insert(ht, new HT::Element("key6666", 7, "payload", 7)))
            cout << "Insert good\n";
        else cout << "Insert error\n";
        HT::printAll(ht);

        HT::Element* ht2 = HT::Get(ht, new HT::Element("key4444", 7));
        if (HT::Delete(ht, ht2))cout << "Good delete\n";
        else cout << "bad delete";
        HT::printAll(ht);
        HT::Element* ht3 = HT::Get(ht, new HT::Element("key2222", 7));
        if (HT::Delete(ht, ht3))cout << "Good delete\n";
        else cout << "bad delete";
        HT::printAll(ht);
        HT::Element* ht4 = HT::Get(ht, new HT::Element("key5555", 7));
        if (HT::Delete(ht, ht4))cout << "Good delete\n";
        else cout << "bad delete";
        HT::printAll(ht);
        HT::Element* ht5 = HT::Get(ht, new HT::Element("key6666", 7));
        if (HT::Delete(ht, ht5))cout << "Good delete\n";
        else cout << "bad delete";
        HT::printAll(ht);

        if (HT::Close(ht))cout << "Close good";
        else throw "Close bad";

        ht = HT::Open(FILE_NAME);
        if (ht)cout << "Open success\n";
        else cout << "Bad open";


        HT::Element* ht12 = HT::Get(ht, new HT::Element("key2222", 7));
        if (ht12)cout << "Get\n";
        else cout << "Get error\n";

        if (HT::Snap(ht))cout << "Good snap\n";
        else cout << "bad snap";

        HT::Element* ht13 = HT::Get(ht, new HT::Element("key2222", 7));
        if (ht13)cout << "Get\n";
        else cout << "Get error\n";


        if (HT::Delete(ht, ht13))cout << "Good delete\n";
        else cout << "bad delete";

        if (HT::Close(ht))cout << "Close good";
        else throw "Close bad";

        ht = HT::Open(FILE_NAME);
        if (ht)cout << "Open success\n";
        else cout << "Bad open";

        if (HT::Insert(ht, new HT::Element("key3333", 7, "payload3", 8)))cout << "Insert good\n";
        else cout << "Insert error\n";

        HT::Element* hte2 = HT::Get(ht, new HT::Element("key2222", 7));
        if (hte2) cout << "Get\n";
        else cout << "Get error\n";

        HT::printAll(ht);

        if (HT::Close(ht))cout << "Close good";
        else throw "Close bad";

        //-------------------------------------------------

        ht22 = HT::Create();
        if (ht)cout << "create succes\n";
        else throw "create error";


        if (HT::Insert(ht22, new HT::Element("key2222", 7, "payload", 7)))
            cout << "Insert good\n";
        else cout << "Insert error\n";

        if (HT::Insert(ht22, new HT::Element("key2222", 7, "payload", 7)))
            cout << "Insert good\n";
        else cout << "Insert error\n";

        if (HT::Insert(ht22, new HT::Element("key3333", 7, "payload", 7)))
            cout << "Insert good\n";
        else cout << "Insert error\n";

        HT::printAll(ht22);

        if (HT::Close(ht22))cout << "Close good";
        else throw "Close bad";
        //-------------------------------------------------
    }
    catch (const char* msg) {
        cout << msg << endl;
    }
    system("pause");
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
