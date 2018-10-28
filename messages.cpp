#include "messages.h"

Serial operator<<(Serial &port, const RequestMessage &req)
{
    port << req.type;
    stream << req.flags;

    stream << revertBytesRet(req.march);
    stream << revertBytesRet(req.lag);
    stream << revertBytesRet(req.depth);
    stream << revertBytesRet(req.roll);
    stream << revertBytesRet(req.pitch);
    stream << revertBytesRet(req.yaw);

    for(int i=0; i<DevAmount; i++) {
        stream << req.dev[i];
    }

    stream << req.dev_flags;
    stream << req.stabilize_flags;
    stream << req.cameras;
    stream << req.pc_reset;
    stream << getCheckSumm16b(stream, req.length); // do i need to revert bytes here?

    return stream;
}

std::iostream& operator<<(std::iostream &stream, const ConfigRequestMessage &req)
{
    //ds.setByteOrder(QDataStream::LittleEndian);
    stream << req.type;

    for(int i=0; i<ControlAmount; i++) {
        stream << revertBytesRet(req.depth_control[i]);
    }

    for(int i=0; i<ControlAmount; i++) {
        stream << revertBytesRet(req.roll_control[i]);
    }

    for(int i=0; i<ControlAmount; i++) {
        stream << revertBytesRet(req.pitch_control[i]);
    }

    for(int i=0; i<ControlAmount; i++) {
        stream << revertBytesRet(req.yaw_control[i]);
    }

    for(int i=0; i<VmaAmount; i++) {
        stream << req.vma_position[i];
    }

    for(int i=0; i<VmaAmount; i++) {
        stream << req.vma_setting[i];
    }

    for(int i=0; i<VmaAmount; i++) {
        stream << req.vma_kforward[i];
    }

    for(int i=0; i<VmaAmount; i++) {
        stream << req.vma_kbackward[i];
    }

    stream << getCheckSumm16b(stream, req.length); // do i need to revert bytes here?

    return stream;
}

std::iostream& operator>>(std::iostream &stream, ResponseMessage &res)
{
    uint16_t checksum_calc = getCheckSumm16b(stream, res.length);

    stream >> res.roll;
    revertBytes(&res.roll);
    stream >> res.pitch;
    revertBytes(&res.pitch);
    stream >> res.yaw;
    revertBytes(&res.yaw);

    stream >> res.rollSpeed;
    revertBytes(&res.rollSpeed);
    stream >> res.pitchSpeed;
    revertBytes(&res.pitchSpeed);
    stream >> res.yawSpeed;
    revertBytes(&res.yawSpeed);

    stream >> res.pressure;
    revertBytes(&res.pressure);

    stream >> res.wf_type;
    stream >> res.wf_tickrate;
    stream >> res.wf_voltage;
    stream >> res.wf_x;
    revertBytes(&res.wf_x);
    stream >> res.wf_y;
    revertBytes(&res.wf_y);

    stream >> res.dev_state;
    stream >> res.leak_data;
    revertBytes(&res.leak_data);
    stream >> res.in_pressure;
    revertBytes(&res.in_pressure);

    for(int i=0; i<VmaAmount; i++) {
        stream >> res.vma_current[i];
        revertBytes(&res.vma_current[i]);
    }

    for(int i=0; i<VmaAmount; i++) {
        stream >> res.vma_velocity[i];
    }

    for(int i=0; i<DevAmount; i++) {
        stream >> res.dev_current[i];
        revertBytes(&res.dev_current[i]);
    }

    stream >> res.vma_errors;
    revertBytes(&res.vma_errors);
    stream >> res.dev_errors;
    stream >> res.pc_errors;

    stream >> res.checksum;

    if(checksum_calc == res.checksum) {
        res.check_passed = true;
    }
    else {
        res.check_passed = false;
    }

    return stream;
}

/** @brief Overloaded revert byte order for 2 byte signed int
  *
  * @param[out]  *var Pointer to the variable to revert.
  */
void revertBytes(int16_t *var)
{
    int8_t tmp = 0;
    int8_t *it = reinterpret_cast<int8_t*>(var);
    tmp = it[0];
    it[0] = it[1];
    it[1] = tmp;
}

/** @brief Overloaded revert byte order for 2 byte unsigned int
  *
  * @param[out]  *var Pointer to the variable to revert.
  */
void revertBytes(uint16_t *var)
{
    uint8_t tmp = 0;
    uint8_t *it = reinterpret_cast<uint8_t*>(var);
    tmp = it[0];
    it[0] = it[1];
    it[1] = tmp;
}

/** @brief Overloaded revert byte order for 4 byte float value
  *
  * @param[out]  *var Pointer to the variable to revert.
  */
void revertBytes(float *var)
{
    float tmp = 0;
    uint8_t *it = reinterpret_cast<uint8_t*>(var);
    uint8_t *it_tmp = reinterpret_cast<uint8_t*>(&tmp);

    for(int i=0; i<4; i++) {
        it_tmp[0] = it[3-i];
    }
    *var = tmp;
}

/** @brief Overloaded revert byte order for 2 byte signed int which returns value
  *
  * @param[in]  *var Variable to revert.
  */
int16_t revertBytesRet(int16_t var)
{
    int8_t tmp = 0;
    int8_t *it = reinterpret_cast<int8_t*>(var);
    tmp = it[0];
    it[0] = it[1];
    it[1] = tmp;
    return var;
}

/** @brief Overloaded revert byte order for 2 byte unsigned int which returns value
  *
  * @param[in]  *var Variable to revert.
  */
uint16_t revertBytesRet(uint16_t var)
{
    uint8_t tmp = 0;
    uint8_t *it = reinterpret_cast<uint8_t*>(var);
    tmp = it[0];
    it[0] = it[1];
    it[1] = tmp;
    return var;
}

/** @brief Overloaded revert byte order for 4 byte float value which returns value
  *
  * @param[in]  *var Variable to revert.
  */
float revertBytesRet(float var)
{
    float tmp = 0;
    uint8_t *it = reinterpret_cast<uint8_t*>(&var);
    uint8_t *it_tmp = reinterpret_cast<uint8_t*>(&tmp);

    for(int i=0; i<4; i++) {
        it_tmp[0] = it[3-i];
    }
    var = tmp;
    return var;
}

/** @brief Gets 16 bit checksum for the content of the stream
  *
  * @param[in]  &msg    Link to the stream
  * @param[in]  length  Length of the message in the stream
  */
uint16_t getCheckSumm16b(std::iostream &msg, uint8_t length)
{
    std::streampos pos = msg.tellg();
    uint16_t crc = 0;

    for(int i=0; i < length - 2; i++) {
        crc = static_cast<uint8_t>((crc >> 8) | (crc << 8));
        crc ^= msg.peek();
        crc ^= static_cast<uint8_t>(crc & 0xff) >> 4;
        crc ^= (crc << 8) << 4;
        crc ^= ((crc & 0xff) << 4) << 1;
    }

    msg.seekg(pos);

    return crc;
}

