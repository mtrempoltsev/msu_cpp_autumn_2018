#include <iostream>
#include <iomanip>

using namespace std;

int64_t Abs(int64_t a) {
    return (a >= 0 ? a : -a);
}

class BigInt {
    const int64_t MAX_CELL = 1000000000000000000;
    class MyList {
    public:
        int64_t c;
        MyList* next = NULL;
        MyList* prev = NULL;
        void push(int64_t a) {
            next = new MyList;
            next->c = a;
            next->prev = this;
        }
        void push(MyList *L) {
            push(L->c);
        }/*
        ~MyList() {
            delete this;
        }*/
    };
    int sz = 0;
    int sgn = 1;
    MyList *bgn = NULL, *en = NULL;
public:
    BigInt() {
        bgn = new MyList;
        bgn->c = 0;
        sz = 1;
        en = bgn;
    }
    BigInt(int a) {
        bgn = new MyList;
        if (a >= 0) {
            bgn->c = a;
        } else {
            sgn = -1;
            bgn->c = -((int64_t) a);
        }
        sz = 1;
        en = bgn;
    }
    BigInt(int64_t a) {
        bgn = new MyList;
        if (a < MAX_CELL) {
            bgn->c = Abs(a);
            if (a < 0) {
                sgn = -1;
            }
            sz = 1;
            en = bgn;
        } else {
            bgn->c = a % MAX_CELL;
            bgn->push(Abs(a / MAX_CELL));
            en = bgn->next;
            if (a < 0) {
                sgn = -1;
            }
            sz = 2;
        }
    }
    BigInt& operator=(const BigInt& B) {
        sz = B.sz;
        sgn = B.sgn;
        en = NULL;
        while (bgn != NULL) {
            MyList *cur = bgn->next;
            delete bgn;
            bgn = cur;
        }
        if (B.bgn != NULL) {
            bgn = new MyList;
            bgn->c = B.bgn->c;
            MyList *cur = bgn, *bcur = B.bgn;
            while (bcur->next != NULL) {
                bcur = bcur->next;
                cur->push(bcur);
                cur = cur->next;
            }
            en = cur;
        }
        return *this;
    }
    BigInt(const BigInt& B) {
        *this = B;
        /*sz = B.sz;
        sgn = B.sgn;
        bgn = NULL;
        if (B.bgn != NULL) {
            bgn = new MyList;
            bgn->c = B.bgn->c;
            MyList *cur = bgn, *bcur = B.bgn;
            while (bcur->next != NULL) {
                bcur = bcur->next;
                cur->push(bcur);
            }
        }*/
    }

    BigInt operator-() const {
        if (sz == 1 && bgn->c == 0) {
            return *this;
        }
        BigInt B = *this;
        B.sgn *= -1;
        return B;
    }
    bool operator==(const BigInt &B) const {
        if (sz != B.sz) {
            return 0;
        }
        MyList *cur = bgn, *bcur = B.bgn;
        while (cur != NULL) {
            if (cur->c != bcur->c) {
                return 0;
            }
            cur = cur->next;
            bcur = bcur->next;
        }
        return 1;
    }
    bool operator!=(const BigInt &B) const {
        return !(*this == B);
    }
    bool operator>(const BigInt &B) const {
        if (sgn > B.sgn) {
            return 1;
        } else if (sgn < B.sgn) {
            return 0;
        }
        if (sz > B.sz) {
            return sgn == 1 ? 1 : 0;
        } else if (sz < B.sz) {
            return sgn == 1 ? 0 : 1;
        }
        MyList *cur = en, *bcur = B.en;
        while (cur != NULL) {
            if (cur->c > bcur->c) {
                return sgn == 1 ? 1 : 0;
            } else if (cur->c < bcur->c) {
                return sgn == 1 ? 0 : 1;
            }
            cur = cur->prev;
            bcur = bcur->prev;
        }
        return 0;
    }
    bool operator<(const BigInt &B) const {
        return B > *this;
    }
    bool operator>=(const BigInt &B) const {
        return (*this > B) || (*this == B);
    }
    bool operator<=(const BigInt &B) const {
        return (B > *this) || (*this == B);
    }
    BigInt operator+(const BigInt& B) const {
        BigInt res;
        res.bgn = new MyList;
        res.sz = 1;
        MyList *cur = bgn, *bcur = B.bgn, *rcur = res.bgn;
        int64_t p = 0;
        while (cur != NULL && bcur != NULL && (cur->next != NULL || bcur->next != NULL)) {
            rcur->c = sgn * (cur->c) + B.sgn * (bcur->c) + p;
            p = rcur->c / MAX_CELL;
            if (rcur->c < 0) {
                p = -1;
                rcur->c += MAX_CELL;
                rcur->c = Abs(rcur->c);
            } else {
                rcur->c = rcur->c % MAX_CELL;
            }
            cur = cur->next;
            bcur = bcur->next;
            rcur->next = new MyList;
            rcur->next->prev = rcur;
            rcur = rcur->next;
            ++res.sz;
        }
        if (cur != NULL && bcur != NULL) {
            rcur->c = sgn * (cur->c) + B.sgn * (bcur->c) + p;
            p = rcur->c / MAX_CELL;
            if (rcur->c < 0) {
                res.sgn = -1;
            } else {
                res.sgn = 1;
            }
            rcur->c = rcur->c % MAX_CELL;
            cur = cur->next;
            bcur = bcur->next;
            rcur->next = new MyList;
            rcur->next->prev = rcur;
            rcur = rcur->next;
            ++res.sz;
        }
        while (cur != NULL) {
            rcur->c = sgn * (cur->c) + p;
            p = rcur->c / MAX_CELL;
            if (rcur->c < 0) {
                p = -1;
                rcur->c += MAX_CELL;
                rcur->c = Abs(rcur->c);
            } else {
                rcur->c = rcur->c % MAX_CELL;
            }
            cur = cur->next;
            rcur->next = new MyList;
            rcur->next->prev = rcur;
            rcur = rcur->next;
            ++res.sz;
        }
        while (bcur != NULL) {
            rcur->c = sgn * (bcur->c) + p;
            p = rcur->c / MAX_CELL;
            if (rcur->c < 0) {
                p = -1;
                rcur->c += MAX_CELL;
                rcur->c = Abs(rcur->c);
            } else {
                rcur->c = rcur->c % MAX_CELL;
            }
            bcur = bcur->next;
            rcur->next = new MyList;
            rcur->next->prev = rcur;
            rcur = rcur->next;
            ++res.sz;
        }
        if (p) {
            rcur->c = p;
        } else {
            rcur = rcur->prev;
            delete rcur->next;
            rcur->next = NULL;
            --res.sz;
        }
        res.en = rcur;
        if (rcur->c < 0) {
            res.sgn = -1;
            rcur->c = Abs(rcur->c);
        } else {
            res.sgn = 1;
        }
        return res;
    }
    BigInt operator-(const BigInt& B) const {
        return (*this) + (-B);
    }
    friend std::ostream& operator<<(std::ostream& out, const BigInt& B) {
        if (B.sgn == -1) {
            out << "-";
        }
        MyList *cur = B.en;
        out << (cur->c);
        cur = cur->prev;
        while (cur != NULL) {
            out << std::setw(18) << setfill('0') << cur->c;
            cur = cur->prev;
        }
        return out;
    }
    ~BigInt() {
        MyList *cur, *p;
        cur = bgn;
        while (cur != NULL) {
            p = cur->next;
            delete cur;
            cur = p;
        }
    }
};
