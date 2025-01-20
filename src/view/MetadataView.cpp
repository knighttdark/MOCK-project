#include "view/MetadataView.h"
#include "common/Exception.h"
#include <iostream>
#include <climits>

int MetadataView::showMenu() {
    MenuRenderer menu("Edit Metadata Menu",
                      {"1. Edit Title", "2. Edit Artist", "3. Edit Album",
                       "4. Edit Genre", "5. Edit Year", "6. Edit Track", "0. Exit Editing"},
                      {1, 2, 3, 4, 5, 6, 0});
    return menu.render();
}

void MetadataView::displayMetadata(const map<string, string>& metadata) {
    const int fieldWidth = 20;   
    const int valueWidth = 40;  
    const int totalWidth = fieldWidth + valueWidth + 5; 

    
    auto drawLine = [&]() {
        cout << "+" << string(fieldWidth + 1, '-')
                  << "+" << string(valueWidth + 1, '-')
                  << "+" << endl;
    };

    
    drawLine();
    cout << "| " << "Field" 
              << string(fieldWidth - 5, ' ') 
              << "| " << "Value" 
              << string(valueWidth - 5, ' ') 
              << " |" << endl;
    drawLine();

    
    for (const auto& field : metadata) {
        string key = field.first;
        string value = field.second;

        
        while (value.length() > valueWidth) {
            
            cout << "| " << key;
            if (key.length() < fieldWidth) {
                cout << string(fieldWidth - key.length(), ' '); 
            }
            cout << "| " << value.substr(0, valueWidth); 
            if (valueWidth > value.substr(0, valueWidth).length()) {
                cout << string(valueWidth - value.substr(0, valueWidth).length(), ' '); 
            }
            cout << " |\n";

            
            value = value.substr(valueWidth);
            key = ""; 
        }

        
        cout << "| " << key;
        if (key.length() < fieldWidth) {
            cout << string(fieldWidth - key.length(), ' '); 
        }
        cout << "| " << value;
        if (value.length() < valueWidth) {
            cout << string(valueWidth - value.length(), ' '); 
        }
        cout << " |\n";
    }

    
    drawLine();
}