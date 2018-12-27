//
// Created by dingwenjiang on 2018/12/28.
//

#include <iostream>
#include "./src/TinyJS.h"
#include "./src/TinyJS_Functions.h"
#include "./src/TinyJS_MathFunctions.h"


using std::cout;
using std::cin;
using std::endl;


void js_print(CScriptVar *v, void *userdata) {
    printf("> %s\n", v->getParameter("text")->getString().c_str());
}

void js_dump(CScriptVar *v, void *userdata) {
    CTinyJS *js = (CTinyJS*)userdata;
    js->root->trace(">  ");
}


int main(){
    CTinyJS *js = new CTinyJS();
    /* add the functions from TinyJS_Functions.cpp */
    registerFunctions(js);
    js->addNative("function print(text)", &js_print, 0);
    js->addNative("function dump()", &js_dump, js);
    try {
        js->execute("var lets_quit = 0; function quit() { lets_quit = 1; }");
        js->execute("print(\"Interactive mode... Type quit(); to exit, or print(...); to print something, or dump() to dump the symbol table!\");");
    } catch (CScriptException *e) {
        printf("ERROR: %s\n", e->text.c_str());
    }
    while (js->evaluate("lets_quit") == "0") {
        char buffer[2048];
        fgets ( buffer, sizeof(buffer), stdin );
        try {
            js->execute(buffer);
        } catch (CScriptException *e) {
            printf("ERROR: %s\n", e->text.c_str());
        }
    }
    delete js;
}