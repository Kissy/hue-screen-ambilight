#include "stdafx.h"
#include "DXGIPointerInfo.h"

DXGIPointerInfo::DXGIPointerInfo (BYTE* pPointerShape, UINT uiPointerShapeBufSize, DXGI_OUTDUPL_FRAME_INFO fi, DXGI_OUTDUPL_POINTER_SHAPE_INFO psi) : m_pPointerShape (pPointerShape),
    m_uiPointerShapeBufSize (uiPointerShapeBufSize),
    m_FI (fi),
    m_PSI (psi) {
}

DXGIPointerInfo::~DXGIPointerInfo() {
    if (m_pPointerShape) {
        delete[] m_pPointerShape;
    }
}

BYTE* DXGIPointerInfo::GetBuffer() {
    return m_pPointerShape;
}

UINT DXGIPointerInfo::GetBufferSize() {
    return m_uiPointerShapeBufSize;
}

DXGI_OUTDUPL_FRAME_INFO& DXGIPointerInfo::GetFrameInfo() {
    return m_FI;
}

DXGI_OUTDUPL_POINTER_SHAPE_INFO& DXGIPointerInfo::GetShapeInfo() {
    return m_PSI;
}