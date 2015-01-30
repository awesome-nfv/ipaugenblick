
#ifndef __USER_GET_BUFFER_CALLBACK__
#define __USER_GET_BUFFER_CALLBACK__

#include "service/ipaugenblick_common/ipaugenblick_common.h"
#include "service/ipaugenblick_service/ipaugenblick_server_side.h"

#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"

extern uint64_t user_on_tx_opportunity_cycles;
extern uint64_t user_on_tx_opportunity_called;
extern uint64_t user_on_tx_opportunity_getbuff_called;
extern uint64_t user_on_tx_opportunity_api_mbufs_sent;
extern uint64_t user_on_tx_opportunity_cannot_get_buff;
extern uint64_t user_rx_mbufs;

static inline __attribute__ ((always_inline)) struct rte_mbuf *user_get_buffer(struct sock *sk,int *copy)
{
    struct rte_mbuf *mbuf, *first = NULL,*prev;
    sock_and_selector_idx_t sock_and_selector_idx;
    unsigned int ringset_idx,i=0;

    user_on_tx_opportunity_getbuff_called++;
    if(sk->sk_socket == NULL)
        return NULL;
    sock_and_selector_idx.u.data = (unsigned long)sk->sk_user_data;
    ringset_idx = RINGSET_IDX(sock_and_selector_idx);
    while(*copy > 1448) {
        mbuf = ipaugenblick_dequeue_tx_buf(ringset_idx);
        if(unlikely(mbuf == NULL)) {
            user_on_tx_opportunity_cannot_get_buff++;
            return first;
        }
        (*copy) -= mbuf->pkt.data_len;
        if(!first)
            first = mbuf;
        else
            prev->pkt.next = mbuf;
        prev = mbuf;
        user_on_tx_opportunity_api_mbufs_sent++;
        break;
    }
    return first;
}

#endif /* __USER_GET_BUFFER_CALLBACK__ */
