#pragma once

#include <JuceHeader.h>
#include <vector>

template <typename ObjTy>
class KeyboardLayout
{
public:
        typedef void(*Callback)(ObjTy* obj, const int key_code);

        KeyboardLayout(ObjTy* obj);

        void add_keypress(const int key_code, Callback c);
        void add_keypress(const int from, const int to, Callback c);
        void key_press(const int key_code) const;
protected:
        struct KeyPressData
        {
                KeyPressData();
                KeyPressData(const int from, const int to, Callback c);
                const int from;
                const int to;
                Callback c;
        };

        ObjTy* obj;
        std::vector<KeyPressData> layout;
};

template <typename ObjTy>
inline KeyboardLayout<ObjTy>::KeyboardLayout(ObjTy* obj)
        :obj(obj)
{
}

template <typename ObjTy>
inline void KeyboardLayout<ObjTy>::add_keypress(const int key_code, Callback c)
{
        add_keypress(key_code, key_code, c);
}

template<typename ObjTy>
inline void KeyboardLayout<ObjTy>::add_keypress(const int from, const int to, Callback c)
{
        layout.push_back(KeyPressData(from, to, c));
}

template <typename ObjTy>
inline void KeyboardLayout<ObjTy>::key_press(const int key_code) const
{
        for (size_t i = 0; i < layout.size(); i++)
                if (key_code >= layout[i].from && key_code <= layout[i].to)
                        layout[i].c(obj, key_code);
}

template <typename ObjTy>
inline KeyboardLayout<ObjTy>::KeyPressData::KeyPressData()
{
        KeyPressData(0, 0, nullptr);
}

template <typename ObjTy>
inline KeyboardLayout<ObjTy>::KeyPressData::KeyPressData(const int from, const int to, Callback c)
        :from(from), to(to), c(c)
{
}
