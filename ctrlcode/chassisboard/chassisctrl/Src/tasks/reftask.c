/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       referee_usart_task.c/h
  * @brief      RM referee system data solve. RM²ÃÅÐÏµÍ³Êý¾Ý´¦Àí
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Nov-11-2019     RM              1. done
  *
  @verbatim
  ==============================================================================

  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#include "main.h"

/**
 * @brief          single byte upacked
 * @param[in]      void
 * @retval         none
 */
/**
 * @brief          µ¥×Ö½Ú½â°ü
 * @param[in]      void
 * @retval         none
 */
static void referee_unpack_fifo_data(void);

extern UART_HandleTypeDef huart3;

uint8_t ref_buf[2][REF_RX_BUF_LENGHT];

fifo_s_t referee_fifo;
uint8_t referee_fifo_buf[REFEREE_FIFO_BUF_LENGTH];
unpack_data_t referee_unpack_obj;

/**
 * @brief          single byte upacked
 * @param[in]      void
 * @retval         none
 */
/**
 * @brief          µ¥×Ö½Ú½â°ü
 * @param[in]      void
 * @retval         none
 */
void referee_unpack_fifo_data(void)
{
    uint8_t byte = 0;
    uint8_t sof = HEADER_SOF;
    unpack_data_t *p_obj = &referee_unpack_obj;

    while (fifo_s_used(&referee_fifo))
    {
        byte = fifo_s_get(&referee_fifo);
        switch (p_obj->unpack_step)
        {
        case STEP_HEADER_SOF:
        {
            if (byte == sof)
            {
                p_obj->unpack_step = STEP_LENGTH_LOW;
                p_obj->protocol_packet[p_obj->index++] = byte;
            }
            else
            {
                p_obj->index = 0;
            }
        }
        break;

        case STEP_LENGTH_LOW:
        {
            p_obj->data_len = byte;
            p_obj->protocol_packet[p_obj->index++] = byte;
            p_obj->unpack_step = STEP_LENGTH_HIGH;
        }
        break;

        case STEP_LENGTH_HIGH:
        {
            p_obj->data_len |= (byte << 8);
            p_obj->protocol_packet[p_obj->index++] = byte;

            if (p_obj->data_len < (REF_PROTOCOL_FRAME_MAX_SIZE - REF_HEADER_CRC_CMDID_LEN))
            {
                p_obj->unpack_step = STEP_FRAME_SEQ;
            }
            else
            {
                p_obj->unpack_step = STEP_HEADER_SOF;
                p_obj->index = 0;
            }
        }
        break;
        case STEP_FRAME_SEQ:
        {
            p_obj->protocol_packet[p_obj->index++] = byte;
            p_obj->unpack_step = STEP_HEADER_CRC8;
        }
        break;

        case STEP_HEADER_CRC8:
        {
            p_obj->protocol_packet[p_obj->index++] = byte;

            if (p_obj->index == REF_PROTOCOL_HEADER_SIZE)
            {
                if (verify_CRC8_check_sum(p_obj->protocol_packet, REF_PROTOCOL_HEADER_SIZE))
                {
                    p_obj->unpack_step = STEP_DATA_CRC16;
                }
                else
                {
                    p_obj->unpack_step = STEP_HEADER_SOF;
                    p_obj->index = 0;
                }
            }
        }
        break;

        case STEP_DATA_CRC16:
        {
            if (p_obj->index < (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
            {
                p_obj->protocol_packet[p_obj->index++] = byte;
            }
            if (p_obj->index >= (REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
            {
                p_obj->unpack_step = STEP_HEADER_SOF;
                p_obj->index = 0;

                if (verify_CRC16_check_sum(p_obj->protocol_packet, REF_HEADER_CRC_CMDID_LEN + p_obj->data_len))
                {
                    referee_data_solve(p_obj->protocol_packet);
                }
            }
        }
        break;

        default:
        {
            p_obj->unpack_step = STEP_HEADER_SOF;
            p_obj->index = 0;
        }
        break;
        }
    }
}

void REF_INIT(void)
{
    DMA_RX_INIT(&huart3, &hdma_usart3_rx, ref_buf[0], ref_buf[1], REF_RX_BUF_LENGHT);
}

void USART3_IRQHandler(void)
{

    if (huart3.Instance->SR & UART_FLAG_RXNE)
    {
        __HAL_UART_CLEAR_PEFLAG(&huart3);
    }
    else if (huart3.Instance->SR & UART_FLAG_IDLE)
    {
        static uint16_t this_time_rx_len = 0;
        __HAL_UART_CLEAR_PEFLAG(&huart3);
        this_time_rx_len = REF_RX_BUF_LENGHT - hdma_usart3_rx.Instance->NDTR;
        hdma_usart3_rx.Instance->NDTR = REF_RX_BUF_LENGHT;
        if ((hdma_usart3_rx.Instance->CR & DMA_SxCR_CT) == RESET)
        {
            hdma_usart3_rx.Instance->CR |= DMA_SxCR_CT;
            fifo_s_puts(&referee_fifo, (char *)ref_buf[0], this_time_rx_len);
            __HAL_DMA_ENABLE(&hdma_usart3_rx);
        }
        else
        {
            DMA1_Stream1->CR &= ~(DMA_SxCR_CT);
            fifo_s_puts(&referee_fifo, (char *)ref_buf[1], this_time_rx_len);
            __HAL_DMA_ENABLE(&hdma_usart3_rx);
        }
    }
}

void reftask()
{
    init_referee_struct_data();
    fifo_s_init(&referee_fifo, referee_fifo_buf, REFEREE_FIFO_BUF_LENGTH);
    // usart6_init(usart6_buf[0], usart6_buf[1], USART_RX_BUF_LENGHT);
    for (;;)
    {
        referee_unpack_fifo_data();
        osDelayUntil(reftaskperi);
    }
}