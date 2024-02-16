#pragma once

template <typename T>
class Ptr
{
private:
    T* Asset;

public:
    T* Get() const { return Asset; }
    T** GetAdressOf() const { return &Asset; }
    T* operator->() const { return Asset; }

public:
    Ptr<T>& operator=(const Ptr& _ptr)
    {
        if (nullptr != Asset)
            Asset->Release();

        Asset = _ptr.Asset;

        if (nullptr != Asset)
            Asset->AddRef();

        return *this;
    }

    Ptr<T>& operator=(T* _Asset)
    {
        if (nullptr != Asset)
            Asset->Release();

        Asset = _Asset;

        if (nullptr != Asset)
            Asset->AddRef();

        return *this;
    }

    bool operator==(const Ptr<T>& _Other) const
    {
        if (Asset == _Other.Asset)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool operator!=(const Ptr<T>& _Other) const { return !(*this == _Other); }

    bool operator==(const T* _Asset) const
    {
        if (Asset == _Asset)
            return true;
        else
            return false;
    }

    bool operator!=(const T* _Asset) const { return !(*this == _Asset); }

public:
    Ptr()
        : Asset(nullptr)
    {
    }

    Ptr(const Ptr<T>& _Other)
        : Asset(_Other.Asset)
    {
        if (nullptr != Asset)
            Asset->AddRef();
    }

    Ptr(T* _Asset)
        : Asset(_Asset)
    {
        if (nullptr != Asset)
            Asset->AddRef();
    }

    ~Ptr()
    {
        if (Asset)
        {
            Asset->Release();
        }
    }
};

template <typename T>
bool operator==(const void* _Asset, const Ptr<T>& _Ptr)
{
    if (_Asset == _Ptr.Get())
        return true;
    else
        return false;
}

template <typename T>
bool operator!=(const void* _Asset, const Ptr<T>& _Ptr)
{
    return !(_Asset == _Ptr);
}