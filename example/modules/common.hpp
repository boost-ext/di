#ifndef COMMON_HPP
#define COMMON_HPP

struct ibutton { virtual ~ibutton() = default; virtual void dummy() = 0; }; struct button : ibutton { void dummy() override { }; };
struct iwindow { virtual ~iwindow() = default; virtual void dummy() = 0; }; struct window : iwindow { void dummy() override { }; };
struct iboard { virtual ~iboard() = default; virtual void dummy() = 0; }; struct board : iboard { void dummy() override { }; };

#endif

