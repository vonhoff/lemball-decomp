#ifndef LEMBALL_NETWORK_NSTREAM_H
#define LEMBALL_NETWORK_NSTREAM_H

struct NETWORK_EffStreamBase {
    void **m_pVtable;
    int m_nReserved04;
    int m_pPayloadBuffer08;
    int m_fOwnsPayload0c;
    int m_nReserved10;
    int m_nReserved14;
    int m_nBufferEnd18;
    int m_nReserved1c;
    int m_nReserved20;
    int m_nReserved24;
    int m_fBusy28;
    int m_pTagBuffer2c;
    unsigned short m_nWord30;
    unsigned short m_nWord32;
    unsigned short m_nWord34;
    unsigned short m_nWord36;

    void DestroyEffStreamBase(void);
    void DestroyEffStreamBaseThunk(void);
};

#endif
