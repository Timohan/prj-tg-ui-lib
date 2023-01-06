/*!
 * \file
 * \brief file tg_global_wait_renderer.cpp
 *
 * wait renderer
 *
 * Copyright of Timo Hannukkala. All rights reserved.
 *
 * \author Timo Hannukkala <timohannukkala@hotmail.com>
 */

#include "tg_global_wait_renderer.h"
#include "../tg_global_macros.h"
#include "../tg_global_log.h"

TgGlobalWaitRenderer *TgGlobalWaitRenderer::m_globalWaitRender = nullptr;

TgGlobalWaitRenderer *TgGlobalWaitRenderer::getInstance()
{
    TG_FUNCTION_BEGIN();
    if (!TgGlobalWaitRenderer::m_globalWaitRender) {
        TgGlobalWaitRenderer::m_globalWaitRender = new TgGlobalWaitRenderer();
    }
    TG_FUNCTION_END();
    return TgGlobalWaitRenderer::m_globalWaitRender;
}

/*!
 * \brief TgGlobalWaitRenderer::waitForRender
 *
 * this is function that waits for next render
 * to make cpu usage slower. function release() can be used
 * to release waiting without timeout
 */
void TgGlobalWaitRenderer::waitForRender()
{
    TG_FUNCTION_BEGIN();
#if DEFAULT_RENDER_WAIT_MAX_TIMEOUT != 0
    if (m_renderCount > 3) {
        m_mutex.lock();
        if (m_nextTimeMaxTimeOut > 0) {
            size_t waitTime = m_nextTimeMaxTimeOut;
            m_nextTimeMaxTimeOut = m_currentTimeMaxTimeOut;
            m_mutex.unlock();
            m_semHandler.try_lock_until(std::chrono::steady_clock::now() + std::chrono::milliseconds(waitTime));
        } else {
            m_nextTimeMaxTimeOut = m_currentTimeMaxTimeOut;
            m_mutex.unlock();
         }
    } else {
        m_renderCount++;
    }
#endif
    TG_FUNCTION_END();
}

/*!
 * \brief TgGlobalWaitRenderer::release
 *
 * \param maxTimeout if not UINT64_MAX, then this time is set as
 * next max timeout in function waitForRender
 */
void TgGlobalWaitRenderer::release(size_t maxTimeout)
{
    TG_FUNCTION_BEGIN();
#if DEFAULT_RENDER_WAIT_MAX_TIMEOUT != 0
    m_mutex.lock();
    m_nextTimeMaxTimeOut = 0;
    if (maxTimeout != UINT64_MAX) {
        m_currentTimeMaxTimeOut = maxTimeout;
    }
    m_mutex.unlock();
    m_semHandler.unlock();
#else
    (void)maxTimeout;
#endif
    TG_FUNCTION_END();
}
