/*.$file${src::philo.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*
* Model: dpp.qm
* File:  ${src::philo.c}
*
* This code has been generated by QM 5.1.0 <www.state-machine.com/qm/>.
* DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
* or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
* for more details.
*/
/*.$endhead${src::philo.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
#include "qpc.h"
#include "dpp.h"
#include "bsp.h"

Q_DEFINE_THIS_MODULE("philo")

/* Active object class -----------------------------------------------------*/
/*.$declare${AOs::Philo} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Philo} ...........................................................*/
typedef struct {
/* protected: */
    QActive super;

/* private: */
    QTimeEvt timeEvt;
} Philo;

/* protected: */
static QState Philo_initial(Philo * const me, void const * const par);
static QState Philo_thinking(Philo * const me, QEvt const * const e);
static QState Philo_hungry(Philo * const me, QEvt const * const e);
static QState Philo_eating(Philo * const me, QEvt const * const e);
/*.$enddecl${AOs::Philo} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/* Local objects -----------------------------------------------------------*/
static Philo l_philo[N_PHILO];   /* storage for all Philos */

#define THINK_TIME  \
    (QTimeEvtCtr)((BSP_random() % BSP_TICKS_PER_SEC) + (BSP_TICKS_PER_SEC/2U))
#define EAT_TIME    \
    (QTimeEvtCtr)((BSP_random() % BSP_TICKS_PER_SEC) + BSP_TICKS_PER_SEC)

/* helper macro to provide the ID of Philo "me_" */
#define PHILO_ID(me_)    ((uint8_t)((me_) - l_philo))

/* Global objects ----------------------------------------------------------*/
/*.$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*. Check for the minimum required QP version */
#if (QP_VERSION < 680U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 6.8.0 or higher required
#endif
/*.$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${AOs::AO_Philo[N_PHILO]} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/

/* opaque pointers to the Philo AOs */
/*.${AOs::AO_Philo[N_PHILO]} ...............................................*/
QActive * const AO_Philo[N_PHILO] = {
    &l_philo[0].super,
    &l_philo[1].super,
    &l_philo[2].super,
    &l_philo[3].super,
    &l_philo[4].super
};
/*.$enddef${AOs::AO_Philo[N_PHILO]} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

/* Philo definition --------------------------------------------------------*/
/*.$define${AOs::Philo_ctor} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Philo_ctor} ......................................................*/
void Philo_ctor(void) {
    uint8_t n;
    Philo *me;
    for (n = 0U; n < N_PHILO; ++n) {
        me = &l_philo[n];
        QActive_ctor(&me->super, Q_STATE_CAST(&Philo_initial));
        QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
    }
}
/*.$enddef${AOs::Philo_ctor} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
/*.$define${AOs::Philo} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv*/
/*.${AOs::Philo} ...........................................................*/
/*.${AOs::Philo::SM} .......................................................*/
static QState Philo_initial(Philo * const me, void const * const par) {
    /*.${AOs::Philo::SM::initial} */
    static uint8_t registered = (uint8_t)0; /* starts off with 0, per C-standard */
    (void)par; /* unused parameter */
    if (registered == (uint8_t)0) {
        registered = (uint8_t)1;

        QS_OBJ_DICTIONARY(&l_philo[0].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[1].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[2].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[3].timeEvt);
        QS_OBJ_DICTIONARY(&l_philo[4].timeEvt);

        QS_FUN_DICTIONARY(&Philo_initial);
        QS_FUN_DICTIONARY(&Philo_thinking);
        QS_FUN_DICTIONARY(&Philo_hungry);
        QS_FUN_DICTIONARY(&Philo_eating);
    }

    QActive_subscribe(&me->super, EAT_SIG);
    QActive_subscribe(&me->super, TEST_SIG);
    return Q_TRAN(&Philo_thinking);
}
/*.${AOs::Philo::SM::thinking} .............................................*/
static QState Philo_thinking(Philo * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${AOs::Philo::SM::thinking} */
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->timeEvt, THINK_TIME, 0U);
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::Philo::SM::thinking} */
        case Q_EXIT_SIG: {
            QTimeEvt_disarm(&me->timeEvt);
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::Philo::SM::thinking::TIMEOUT} */
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Philo_hungry);
            break;
        }
        /*.${AOs::Philo::SM::thinking::EAT, DONE} */
        case EAT_SIG: /* intentionally fall through */
        case DONE_SIG: {
            /* EAT or DONE must be for other Philos than this one */
            Q_ASSERT_ID(10, Q_EVT_CAST(TableEvt)->philoNum != PHILO_ID(me));
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*.${AOs::Philo::SM::hungry} ...............................................*/
static QState Philo_hungry(Philo * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${AOs::Philo::SM::hungry} */
        case Q_ENTRY_SIG: {
            TableEvt *pe = Q_NEW(TableEvt, HUNGRY_SIG);
            pe->philoNum = PHILO_ID(me);
            QACTIVE_POST(AO_Table, &pe->super, me);
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::Philo::SM::hungry::EAT} */
        case EAT_SIG: {
            /*.${AOs::Philo::SM::hungry::EAT::[Q_EVT_CAST(TableEvt)->philoNum=~} */
            if (Q_EVT_CAST(TableEvt)->philoNum == PHILO_ID(me)) {
                status_ = Q_TRAN(&Philo_eating);
            }
            else {
                status_ = Q_UNHANDLED();
            }
            break;
        }
        /*.${AOs::Philo::SM::hungry::DONE} */
        case DONE_SIG: {
            /* DONE must be for other Philos than this one */
            Q_ASSERT_ID(20, Q_EVT_CAST(TableEvt)->philoNum != PHILO_ID(me));
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*.${AOs::Philo::SM::eating} ...............................................*/
static QState Philo_eating(Philo * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        /*.${AOs::Philo::SM::eating} */
        case Q_ENTRY_SIG: {
            QTimeEvt_armX(&me->timeEvt, EAT_TIME, 0U);
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::Philo::SM::eating} */
        case Q_EXIT_SIG: {
            TableEvt *pe = Q_NEW(TableEvt, DONE_SIG);
            pe->philoNum = PHILO_ID(me);
            QF_PUBLISH(&pe->super, &me->super);
            QTimeEvt_disarm(&me->timeEvt);
            status_ = Q_HANDLED();
            break;
        }
        /*.${AOs::Philo::SM::eating::TIMEOUT} */
        case TIMEOUT_SIG: {
            status_ = Q_TRAN(&Philo_thinking);
            break;
        }
        /*.${AOs::Philo::SM::eating::EAT, DONE} */
        case EAT_SIG: /* intentionally fall through */
        case DONE_SIG: {
            /* EAT or DONE must be for other Philos than this one */
            Q_ASSERT_ID(30, Q_EVT_CAST(TableEvt)->philoNum != PHILO_ID(me));
            status_ = Q_HANDLED();
            break;
        }
        default: {
            status_ = Q_SUPER(&QHsm_top);
            break;
        }
    }
    return status_;
}
/*.$enddef${AOs::Philo} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
