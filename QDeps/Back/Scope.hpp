template<template<typename> class TAllocator>
class Scope
{
    template<typename T> struct ResultType
    {
        typedef TAllocator<T> type;
    };
};

