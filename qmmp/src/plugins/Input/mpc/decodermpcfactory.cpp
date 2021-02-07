#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpcfile.h>
#include <taglib/apetag.h>
#include <taglib/tfilestream.h>
#include "mpcmetadatamodel.h"
#include "decoder_mpc.h"
#include "decodermpcfactory.h"

bool DecoderMPCFactory::canDecode(QIODevice *input) const
{
    char buf[36];
    if(input->peek(buf, 4) != 4)
        return false;

    if(!memcmp(buf, "MP+", 3) || !memcmp(buf, "MPCK", 4))
        return true;

    return false;
}

DecoderProperties DecoderMPCFactory::properties() const
{
    DecoderProperties properties;
    properties.name = tr("Musepack Plugin");
    properties.filters << "*.mpc";
    properties.description = tr("Musepack Files");
    properties.shortName = "mpc";
    return properties;
}

Decoder *DecoderMPCFactory::create(const QString &path, QIODevice *input)
{
    Q_UNUSED(path);
    return new DecoderMPC(input);
}

QList<TrackInfo*> DecoderMPCFactory::createPlayList(const QString &path, TrackInfo::Parts parts, QStringList *)
{
    TrackInfo *info = new TrackInfo(path);

    if(parts == TrackInfo::Parts())
    {
        return QList<TrackInfo*>() << info;
    }

    TagLib::FileStream stream(QStringToFileName(path), true);
    TagLib::MPC::File fileRef(&stream);

    if((parts & TrackInfo::MetaData) && fileRef.APETag() && !fileRef.APETag()->isEmpty())
    {
        TagLib::APE::Tag *tag = fileRef.APETag();
        info->setValue(Qmmp::ALBUM, TStringToQString(tag->album()));
        info->setValue(Qmmp::ARTIST, TStringToQString(tag->artist()));
        info->setValue(Qmmp::COMMENT, TStringToQString(tag->comment()));
        info->setValue(Qmmp::GENRE, TStringToQString(tag->genre()));
        info->setValue(Qmmp::TITLE, TStringToQString(tag->title()));
        info->setValue(Qmmp::YEAR, tag->year());
        info->setValue(Qmmp::TRACK, tag->track());
        TagLib::APE::Item fld;
        if(!(fld = tag->itemListMap()["ALBUM ARTIST"]).isEmpty())
            info->setValue(Qmmp::ALBUMARTIST, TStringToQString(fld.toString()));
        if(!(fld = tag->itemListMap()["COMPOSER"]).isEmpty())
            info->setValue(Qmmp::COMPOSER, TStringToQString(fld.toString()));
    }

    TagLib::MPC::Properties *ap = fileRef.audioProperties();
    if((parts & TrackInfo::Properties) && ap)
    {
        info->setValue(Qmmp::BITRATE, ap->bitrate());
        info->setValue(Qmmp::SAMPLERATE, ap->sampleRate());
        info->setValue(Qmmp::CHANNELS, ap->channels());
        info->setValue(Qmmp::BITS_PER_SAMPLE, 16);
        info->setValue(Qmmp::FORMAT_NAME, QString("Musepack SV%1").arg(ap->mpcVersion()));
        info->setDuration(ap->lengthInMilliseconds());
    }

    return QList<TrackInfo*>() << info;
}

MetaDataModel* DecoderMPCFactory::createMetaDataModel(const QString &path, bool readOnly)
{
    return new MPCMetaDataModel(path, readOnly);
}

#ifndef QMMP_GREATER_NEW
#include <QtPlugin>
Q_EXPORT_PLUGIN2(mpc, DecoderMPCFactory)
#endif