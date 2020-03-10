/**
 * SDRangel
 * This is the web REST/JSON API of SDRangel SDR software. SDRangel is an Open Source Qt5/OpenGL 3.0+ (4.3+ in Windows) GUI and server Software Defined Radio and signal analyzer in software. It supports Airspy, BladeRF, HackRF, LimeSDR, PlutoSDR, RTL-SDR, SDRplay RSP1 and FunCube    ---   Limitations and specifcities:    * In SDRangel GUI the first Rx device set cannot be deleted. Conversely the server starts with no device sets and its number of device sets can be reduced to zero by as many calls as necessary to /sdrangel/deviceset with DELETE method.   * Preset import and export from/to file is a server only feature.   * Device set focus is a GUI only feature.   * The following channels are not implemented (status 501 is returned): ATV and DATV demodulators, Channel Analyzer NG, LoRa demodulator   * The device settings and report structures contains only the sub-structure corresponding to the device type. The DeviceSettings and DeviceReport structures documented here shows all of them but only one will be or should be present at a time   * The channel settings and report structures contains only the sub-structure corresponding to the channel type. The ChannelSettings and ChannelReport structures documented here shows all of them but only one will be or should be present at a time    --- 
 *
 * OpenAPI spec version: 4.13.0
 * Contact: f4exb06@gmail.com
 *
 * NOTE: This class is auto generated by the swagger code generator program.
 * https://github.com/swagger-api/swagger-codegen.git
 * Do not edit the class manually.
 */

/*
 * SWGChannelActions.h
 *
 * Base channel actions. Only the channel actions corresponding to the channel specified in the channelType field is or should be present.
 */

#ifndef SWGChannelActions_H_
#define SWGChannelActions_H_

#include <QJsonObject>


#include "SWGFileSourceActions.h"
#include <QString>

#include "SWGObject.h"
#include "export.h"

namespace SWGSDRangel {

class SWG_API SWGChannelActions: public SWGObject {
public:
    SWGChannelActions();
    SWGChannelActions(QString* json);
    virtual ~SWGChannelActions();
    void init();
    void cleanup();

    virtual QString asJson () override;
    virtual QJsonObject* asJsonObject() override;
    virtual void fromJsonObject(QJsonObject &json) override;
    virtual SWGChannelActions* fromJson(QString &jsonString) override;

    QString* getChannelType();
    void setChannelType(QString* channel_type);

    qint32 getDirection();
    void setDirection(qint32 direction);

    qint32 getOriginatorDeviceSetIndex();
    void setOriginatorDeviceSetIndex(qint32 originator_device_set_index);

    qint32 getOriginatorChannelIndex();
    void setOriginatorChannelIndex(qint32 originator_channel_index);

    SWGFileSourceActions* getFileSourceActions();
    void setFileSourceActions(SWGFileSourceActions* file_source_actions);


    virtual bool isSet() override;

private:
    QString* channel_type;
    bool m_channel_type_isSet;

    qint32 direction;
    bool m_direction_isSet;

    qint32 originator_device_set_index;
    bool m_originator_device_set_index_isSet;

    qint32 originator_channel_index;
    bool m_originator_channel_index_isSet;

    SWGFileSourceActions* file_source_actions;
    bool m_file_source_actions_isSet;

};

}

#endif /* SWGChannelActions_H_ */
