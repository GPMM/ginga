/* Copyright (C) 2006-2017 PUC-Rio/Laboratorio TeleMidia

This file is part of Ginga (Ginga-NCL).

Ginga is free software: you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

Ginga is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
License for more details.

You should have received a copy of the GNU General Public License
along with Ginga.  If not, see <http://www.gnu.org/licenses/>.  */

#include "ginga.h"
#include "FormatterMultiDevice.h"

#include "FormatterMediator.h"
#include "FormatterBaseDevice.h"

#if WITH_MULTIDEVICE
# include "FormatterPassiveDevice.h"
# include "FormatterActiveDevice.h"
#endif

#include "mb/DisplayManager.h"
#include "mb/InputManager.h"
#include "mb/CodeMap.h"
using namespace ::ginga::mb;

#include "ncl/DeviceLayout.h"
using namespace ::ginga::ncl;

GINGA_FORMATTER_BEGIN

#if WITH_MULTIDEVICE
RemoteDeviceManager *FormatterMultiDevice::rdm = NULL;
#else
void *FormatterMultiDevice::rdm = NULL;
#endif

FormatterMultiDevice::FormatterMultiDevice (DeviceLayout *deviceLayout,
                                            int x, int y, int w, int h,
                                            bool useMulticast, arg_unused (int srvPort))
{
  this->xOffset = x;
  this->yOffset = y;
  this->defaultWidth = w;
  this->defaultHeight = h;
  this->deviceClass = -1;
  this->hasRemoteDevices = false;
  this->deviceLayout = deviceLayout;
  this->activeBaseUri = "";
  this->activeUris = NULL;
  this->bitMapScreen = 0;
  this->serialized = 0;
  this->presContext = NULL;
  this->focusManager = NULL;
  this->parent = NULL;
  this->enableMulticast = useMulticast;

  DisplayManager::addIEListenerInstance (this);

  if (defaultWidth == 0)
    {
      defaultWidth = Ginga_Display->getWidthResolution ();
    }

  if (defaultHeight == 0)
    {
      defaultHeight = Ginga_Display->getHeightResolution ();
    }

  im = Ginga_Display_M->getInputManager ();

  im->setAxisValues ((int)(Ginga_Display->getWidthResolution () / 2),
                     (int)(Ginga_Display->getHeightResolution () / 2), 0);

  printScreen = Ginga_Display_M->createWindow (0, 0,
                                                defaultWidth,
                                                defaultHeight, -1.0);

  int caps = Ginga_Display_M->getWindowCap (printScreen, "ALPHACHANNEL");
  Ginga_Display_M->setWindowCaps (printScreen, caps);
  Ginga_Display_M->drawWindow (printScreen);

  Thread::mutexInit (&mutex, false);
  Thread::mutexInit (&lMutex, false);
}

FormatterMultiDevice::~FormatterMultiDevice ()
{
  set<IPlayer *>::iterator i;
  DisplayManager::removeIEListenerInstance (this);

  presContext = NULL;

  clog << "FormatterMultiDevice::~FormatterMultiDevice ";
  clog << "checking listening list";
  clog << endl;

  if (!listening.empty ())
    {
      Thread::mutexLock (&lMutex);
      /*			i = listening.begin();
                              while (i != listening.end()) {
                                      (*i)->removeListener(this);
                                      ++i;
                              }*/
      listening.clear ();
      Thread::mutexUnlock (&lMutex);
    }

  clog << "FormatterMultiDevice::~FormatterMultiDevice ";
  clog << "destroying mutexes";
  clog << endl;

  Thread::mutexDestroy (&mutex);
  Thread::mutexDestroy (&lMutex);

  /*if (im != NULL) {
          clog << "FormatterMultiDevice::~FormatterMultiDevice ";
          clog << "stop listening events";
          clog << endl;
          im->removeInputEventListener(this);
  }*/

  clog << "FormatterMultiDevice::~FormatterMultiDevice ";
  clog << "all done";
  clog << endl;
}

void
FormatterMultiDevice::printGingaWindows ()
{
  string fileUri = "";
  GingaWindowID iWin;
  NclFormatterLayout *formatterLayout;

  vector<GingaWindowID> sortedIds;
  map<int, NclFormatterLayout *>::iterator i;
  vector<GingaWindowID>::iterator j;

  int quality = 100;
  int dumpW = defaultWidth;
  int dumpH = defaultHeight;

  cout << "FormatterMultiDevice::printGingaWindows(" << this << ") ";
  cout << "layout manager has '" << layoutManager.size ();
  cout << "' layouts" << endl;

  cout << "Serialized window Id = '";
  cout << (unsigned long)serialized;
  cout << "'" << endl;

  Ginga_Display_M->getWindowDumpFileUri (serialized, quality, dumpW, dumpH);

  cout << "BitMapScreen window Id = '";
  if (bitMapScreen != 0)
    {
      cout << (unsigned long)bitMapScreen << "'";
      Ginga_Display_M->getWindowDumpFileUri (bitMapScreen, quality, dumpW,
                                dumpH);
    }
  else
    {
      cout << "NULL'";
    }
  cout << endl;

  i = layoutManager.begin ();
  while (i != layoutManager.end ())
    {
      formatterLayout = i->second;

      cout << "device '" << i->first << "' ";

      formatterLayout->getSortedIds (&sortedIds);
      if (!sortedIds.empty ())
        {
          if (i->first == 1)
            {
              quality = 45;
              dumpW = (int)(480 / 1.8);
              dumpH = (int)(320 / 1.8);
            }

          cout << "has the following : ";
          j = sortedIds.begin ();
          while (j != sortedIds.end ())
            {
              iWin = (*j);

              if (iWin != 0)
                {
                  Ginga_Display_M->getWindowDumpFileUri (iWin, quality, dumpW,
                                            dumpH);
                }

              cout << "'" << (unsigned long)(*j) << "' ";
              ++j;
            }
          cout << endl;
        }
      else
        {
          cout << "is empty " << endl;
        }

      sortedIds.clear ();
      ++i;
    }

  cout << "FormatterMultiDevice::printGingaWindows all done";
  cout << endl;
}

void
FormatterMultiDevice::listenPlayer (IPlayer *player)
{
  Thread::mutexLock (&lMutex);
  listening.insert (player);
  player->addListener (this);
  Thread::mutexUnlock (&lMutex);
}

void
FormatterMultiDevice::stopListenPlayer (IPlayer *player)
{
  set<IPlayer *>::iterator i;

  Thread::mutexLock (&lMutex);
  i = listening.find (player);
  if (i != listening.end ())
    {
      listening.erase (i);
      player->removeListener (this);
    }
  Thread::mutexUnlock (&lMutex);
}

void
FormatterMultiDevice::setParent (FormatterMultiDevice *parent)
{
  this->parent = (FormatterMultiDevice *)parent;
}

void
FormatterMultiDevice::setPresentationContex (
    PresentationContext *presContext)
{
  this->presContext = presContext;
}

void
FormatterMultiDevice::setFocusManager (void *focusManager)
{
  this->focusManager = focusManager;
}

void *
FormatterMultiDevice::getMainLayout ()
{
  return mainLayout;
}

void *
FormatterMultiDevice::getFormatterLayout (int devClass)
{
  map<int, NclFormatterLayout *>::iterator i;

  i = layoutManager.find (devClass);
  if (i != layoutManager.end ())
    {
      return i->second;
    }

  return NULL;
}

string
FormatterMultiDevice::getScreenShot ()
{
  return serializeScreen (deviceClass, printScreen);
}

string
FormatterMultiDevice::serializeScreen (int devClass,
                                       GingaWindowID mapWindow)
{
  string fileUri = "";
  NclFormatterLayout *formatterLayout;
  vector<GingaWindowID> sortedIds;
  map<int, NclFormatterLayout *>::iterator i;
  int quality = 100;
  int dumpW = defaultWidth;
  int dumpH = defaultHeight;

  i = layoutManager.find (devClass);
  if (i != layoutManager.end ())
    {
      formatterLayout = i->second;
      Ginga_Display_M->clearWindowContent (mapWindow);
      formatterLayout->getSortedIds (&sortedIds);
      if (!sortedIds.empty ())
        {
          if (!Ginga_Display_M->mergeIds (mapWindow, &sortedIds))
            {
              return "";
            }

          if (devClass == 1)
            {
              quality = 45;
              dumpW = (int)(480 / 1.8);
              dumpH = (int)(320 / 1.8);
            }
        }
      fileUri = Ginga_Display_M->getWindowDumpFileUri (mapWindow, quality,
                                          dumpW, dumpH);

      clog << "FormatterMultiDevice::serializeScreen fileURI = '";
      clog << fileUri << "' sortedIds size = '" << sortedIds.size ();
      clog << endl;
    }

  return fileUri;
}

void
FormatterMultiDevice::postMediaContent (int destDevClass)
{
  string fileUri;
  GingaWindowID bmpScr = 0;
  vector<GingaWindowID> wins;

  /*clog << "FormatterMultiDevice::postMediaContent to class '";
  clog << destDevClass << "'";
  clog << endl;*/

  Thread::mutexLock (&mutex);
  if (destDevClass == DeviceDomain::CT_PASSIVE)
    {
      if (parent != NULL)
        {
          bmpScr = parent->bitMapScreen;
        }
      else
        {
          bmpScr = bitMapScreen;
        }

      fileUri = serializeScreen (destDevClass, serialized);
      if (fileUri != "" && fileExists (fileUri))
        {
#if WITH_MULTIDEVICE
          rdm->postMediaContent (destDevClass, fileUri);

          clog << "FormatterMultiDevice::postMediaContent(";
          clog << this << ")";
          clog << " serialized window id = '";
          clog << (unsigned long)serialized;

#endif // WITH_MULTIDEVICE

          if (bmpScr != 0)
            {
              wins.push_back (serialized);
              Ginga_Display_M->mergeIds (bmpScr, &wins);

              Ginga_Display_M->showWindow (bmpScr);

              clog << "' bmpScr = '";
              clog << (unsigned long)bmpScr;

              /*bmpScr->clearContent();
              bmpScr->stretchBlit(serialized);
              bmpScr->show();
              bmpScr->validate();*/

              // renderFromUri(bitMapScreen, fileUri);
            }

          clog << "'" << endl;
        }
    }
  else if (destDevClass == DeviceDomain::CT_ACTIVE)
    {
      if (!activeUris->empty ())
        {
        }
    }
  Thread::mutexUnlock (&mutex);
}

NclFormatterLayout *
FormatterMultiDevice::getFormatterLayout (
    NclCascadingDescriptor *descriptor, NclExecutionObject *object)
{
  map<int, NclFormatterLayout *>::iterator i;
  NclFormatterLayout *layout;
  LayoutRegion *region;
  int devClass;

  /*clog << "FormatterMultiDevice::getFormatterLayout for '";
  clog << object->getId() << "' formatterMultiDevice class = '";
  clog << this->deviceClass << "'" << endl;*/

  region = descriptor->getRegion ();
  if (region == NULL)
    {
      if (layoutManager.count (this->deviceClass) != 0)
        {
          region = descriptor->getRegion (layoutManager[this->deviceClass],
                                          object);
        }

      if (region == NULL)
        {
          clog << "FormatterMultiDevice::getFormatterLayout ";
          clog << "region is NULL";
          clog << endl;
          return NULL;
        }
    }

  devClass = region->getDeviceClass ();
  i = layoutManager.find (devClass);

  if (i == layoutManager.end ())
    {
      if (devClass == DeviceDomain::CT_PASSIVE)
        {
          layout = new NclFormatterLayout (0, 0, DV_QVGA_WIDTH,
                                           DV_QVGA_HEIGHT);

          layoutManager[devClass] = layout;
          return layout;
        }
      return NULL;
    }
  else
    {
      return i->second;
    }
}

GingaWindowID
FormatterMultiDevice::prepareFormatterRegion (
    NclExecutionObject *executionObject, GingaSurfaceID renderedSurface)
{
  NclFormatterLayout *layout;
  NclCascadingDescriptor *descriptor;
  string regionId, plan = "";
  GingaWindowID windowId = 0;

  map<int, NclFormatterLayout *>::iterator i;
  LayoutRegion *bitMapRegion;
  LayoutRegion *ncmRegion;

  descriptor = executionObject->getDescriptor ();
  if (descriptor != NULL)
    {
      layout = getFormatterLayout (descriptor, executionObject);
      if (layout != NULL)
        {
          if (descriptor->getFormatterRegion () != NULL)
            {
              plan = descriptor->getFormatterRegion ()->getPlan ();
            }

          windowId = layout->prepareFormatterRegion (executionObject,
                                                     renderedSurface, plan);

          if (bitMapScreen != 0)
            {
              /*clog << endl;
              clog << "FormatterMultiDevice::prepareFormatterRegion ";
              clog << "bitMapScreen != NULL" << endl;*/
              return windowId;
            }

          regionId = layout->getBitMapRegionId ();
          /*clog << endl;
          clog << "FormatterMultiDevice::prepareFormatterRegion map '";
          clog << regionId << "'" << endl;*/

          if (regionId == "")
            {
              return windowId;
            }

          i = layoutManager.find (1);
          if (i == layoutManager.end ())
            {
              /*clog << endl;
              clog << "FormatterMultiDevice::prepareFormatterRegion ";
              clog << "CANT FIND devClass '" << deviceClass << "'";
              clog << endl;*/
              return windowId;
            }

          layout = i->second;

          ncmRegion = layout->getDeviceRegion ();

          bitMapRegion = ncmRegion->getOutputMapRegion ();
          if (bitMapRegion == NULL)
            {
              clog << endl;
              clog << "FormatterMultiDevice::prepareFormatterRegion(";
              clog << this << ") ";
              clog << "CANT FIND bitMapRegion";
              clog << " for id '" << regionId << "' devClass = '";
              clog << deviceClass << "'" << endl;
              clog << endl;
              return windowId;
            }

          bitMapScreen = Ginga_Display_M->createWindow (
              bitMapRegion->getAbsoluteLeft (),
              bitMapRegion->getAbsoluteTop (),
              bitMapRegion->getWidthInPixels (),
              bitMapRegion->getHeightInPixels (),
              bitMapRegion->getZIndexValue ());

          clog << endl << endl;
          clog << "FormatterMultiDevice::prepareFormatterRegion(";
          clog << this << ") ";
          clog << "BITMAPREGION '";
          clog << regionId << "' left = '";
          clog << bitMapRegion->getLeftInPixels ();
          clog << "' top = '" << bitMapRegion->getTopInPixels ();
          clog << "' width = '" << bitMapRegion->getWidthInPixels ();
          clog << "' height = '" << bitMapRegion->getHeightInPixels ();
          clog << "' zIndex = '" << bitMapRegion->getZIndexValue ();
          clog << endl << endl;

          int caps
              = Ginga_Display_M->getWindowCap (bitMapScreen, "ALPHACHANNEL");
          Ginga_Display_M->setWindowCaps (bitMapScreen, caps);
          Ginga_Display_M->drawWindow (bitMapScreen);
        }
    }

  return windowId;
}

void
FormatterMultiDevice::showObject (NclExecutionObject *executionObject)
{
  NclFormatterLayout *layout;
  NclCascadingDescriptor *descriptor;
  NclFormatterRegion *fRegion;
  LayoutRegion *region;
  int devClass;
  string fileUri;
  string url;
  string relativePath;
  string tempRelPath;
  string value;
  Content *content;

  /*INCLSectionProcessor* nsp = NULL;
  vector<StreamData*>* streams;*/

  descriptor = executionObject->getDescriptor ();
  if (descriptor != NULL)
    {
      region = descriptor->getRegion ();
      layout = getFormatterLayout (descriptor, executionObject);
      if (region != NULL && layout != NULL)
        {
          devClass = region->getDeviceClass ();

          clog << "FormatterMultiDevice::showObject '";
          clog << executionObject->getId () << "' class '";
          clog << devClass << "'" << endl;

          fRegion = descriptor->getFormatterRegion ();
          if (devClass != DeviceDomain::CT_BASE)
            {
              clog << "FormatterMultiDevice::showObject as base" << endl;

              if (fRegion != NULL)
                {
                  fRegion->setGhostRegion (true);
                }
            }

          if (devClass != DeviceDomain::CT_ACTIVE)
            {
              layout->showObject (executionObject);
            }

          if (hasRemoteDevices)
            {
              if (devClass == DeviceDomain::CT_PASSIVE)
                {
                  postMediaContent (devClass);
                }
              else if (devClass == DeviceDomain::CT_ACTIVE)
                {
                  // clog << "activeBaseUri: "<<activeBaseUri<<endl;
                  // clog << "activeUris: "<<activeUris<<endl;

                  content
                      = ((NodeEntity *)(executionObject->getDataObject ()
                                            ->getDataEntity ()))
                            ->getContent ();

                  tempRelPath = "";

                  if (content != NULL
                      && content->instanceOf ("ReferenceContent"))
                    {
                      url = ((ReferenceContent *)content)
                                ->getCompleteReferenceUrl ();

                      clog << "FormatterMultiDevice::showObject ";
                      clog << "executionObject.url = '" << url;
                      clog << "'" << endl;

                      clog << "FormatterMultiDevice::showObject ";
                      clog << "executionObject.activeBaseUri = '";
                      clog << activeBaseUri << "'" << endl;

                      size_t pos
                          = url.find_last_of (SystemCompat::getIUriD ());

                      if (pos != string::npos)
                        tempRelPath = url.substr (
                            activeBaseUri.size (),
                            url.size () - activeBaseUri.size ());
                      else
                        tempRelPath = url;

                      // relativePath =
                      // SystemCompat::convertRelativePath(tempRelPath);

                      /*
                      size_t pos =
                      url.find_last_of(SystemCompat::getIUriD());
                      if(pos != string::npos)
                              relativePath = url.substr( pos + 1, url.size()
                      -
                      pos - 1 );
                      else
                              relativePath = url;
                      */
                      clog << "FormatterMultiDevice::showObject ";
                      clog << "executionObject.RP = '";
                      clog << tempRelPath << "'" << endl;
                    }
#if WITH_MULTIDEVICE
                  rdm->postEvent (
                      devClass, DeviceDomain::FT_PRESENTATIONEVENT,
                      deconst (char *, ("start::" + tempRelPath).c_str ()),
                      (int)(("start::" + tempRelPath).size ()));
#endif // WITH_MULTIDEVICE
                }
            }
        }
    }
}

void
FormatterMultiDevice::hideObject (NclExecutionObject *executionObject)
{
  NclFormatterLayout *layout;
  NclCascadingDescriptor *descriptor;
  LayoutRegion *region;
  int devClass;
  string fileUri;

  clog << "FormatterMultiDevice::hideObject '";
  clog << executionObject->getId () << "'" << endl;

  descriptor = executionObject->getDescriptor ();
  if (descriptor != NULL)
    {
      region = descriptor->getRegion ();
      layout = getFormatterLayout (descriptor, executionObject);
      if (region != NULL && layout != NULL)
        {
          devClass = region->getDeviceClass ();
          if (devClass != DeviceDomain::CT_ACTIVE)
            {
              /*clog << "FormatterMultiDevice::hideObject '";
              clog << executionObject->getId() << "' class '";
              clog << devClass << "'" << endl;*/

              layout->hideObject (executionObject);
            }

          if (hasRemoteDevices)
            {
              if (devClass == DeviceDomain::CT_PASSIVE)
                {
                  postMediaContent (devClass);
                }
              else if (devClass == DeviceDomain::CT_ACTIVE)
                {
                  Content *content;
                  string relativePath = "";
                  string url;

                  content
                      = ((NodeEntity *)(executionObject->getDataObject ()
                                            ->getDataEntity ()))
                            ->getContent ();

                  if (content != NULL
                      && content->instanceOf ("ReferenceContent"))
                    {
                      url = ((ReferenceContent *)content)
                                ->getCompleteReferenceUrl ();

                      /*clog << "FormatterMultiDevice::hideObject";
                      clog << " executionObject.url = '" << url << "'";
                      clog << " activeBaseUri = '" << activeBaseUri;
                      clog << "'";
                      clog << endl;*/

                      relativePath = url.substr (
                          activeBaseUri.size () + 1,
                          url.size () - activeBaseUri.size ());
                    }
#if WITH_MULTIDEVICE
                  rdm->postEvent (
                      devClass, DeviceDomain::FT_PRESENTATIONEVENT,
                      deconst (char *, ("stop::" + relativePath).c_str ()),
                      (int)("stop::" + relativePath).size ());
#endif // WITH_MULTIDEVICE
                }
            }
        }
    }
}

void
FormatterMultiDevice::renderFromUri (GingaWindowID win, string uri)
{
  GingaSurfaceID s;
  s = Ginga_Display_M->createRenderedSurfaceFromImageFile (uri.c_str ());
  Ginga_Display_M->setWindowColorKey (win, 0, 0, 0);
  Ginga_Display_M->clearWindowContent (win);
  Ginga_Display_M->renderWindowFrom (win, s);
  Ginga_Display_M->showWindow (win);
  Ginga_Display_M->validateWindow (win);
  Ginga_Display_M->deleteSurface (s);
}

void
FormatterMultiDevice::tapObject (int devClass, int x, int y)
{
  NclFormatterLayout *layout;
  NclExecutionObject *object;

  if (layoutManager.count (devClass) != 0)
    {
      layout = layoutManager[devClass];

      object = layout->getObject (x, y);
      if (focusManager != NULL && object != NULL)
        {
          clog << "FormatterMultiDevice::tapObject '";
          clog << object->getId () << "'" << endl;
          ((FormatterFocusManager *)focusManager)
              ->tapObject ((void *)object);
        }
      else
        {
          clog << "FormatterMultiDevice::tapObject can't ";
          clog << "find object at '" << x << "' and '";
          clog << y << "' coords" << endl;
        }
    }
  else
    {
      clog << "FormatterMultiDevice::tapObject can't find layout of '";
      clog << devClass << "' device class" << endl;
    }
}

bool
FormatterMultiDevice::newDeviceConnected (int newDevClass, int w, int h)
{
  bool isNewClass = false;

  clog << "FormatterMultiDevice::newDeviceConnected class '";
  clog << newDevClass << "', w = '" << w << "', h = '" << h << "'";
  clog << endl;

  if (presContext != NULL)
    {
      presContext->incPropertyValue (SYSTEM_DEVNUMBER
                                     + xstrbuild ("(%d)", newDevClass));
    }

  if (!hasRemoteDevices)
    {
      hasRemoteDevices = true;
    }

  if (layoutManager.count (newDevClass) == 0)
    {
      layoutManager[newDevClass]
          = new NclFormatterLayout (0, 0, w, h);

      isNewClass = true;
    }

  if (newDevClass == DeviceDomain::CT_ACTIVE)
    {
      clog << "FormatterMulDevice::newDeviceConnected class = ";
      clog << DeviceDomain::CT_ACTIVE << endl;
    }
  else
    {
      postMediaContent (newDevClass);
    }

  return isNewClass;
}

bool
FormatterMultiDevice::receiveRemoteEvent (int remoteDevClass, int eventType,
                                          string eventContent)
{
  vector<string> *params;
  int eventCode;

  if (remoteDevClass == DeviceDomain::CT_PASSIVE
      && eventType == DeviceDomain::FT_SELECTIONEVENT)
    {
      if (eventContent.find (",") != std::string::npos)
        {
          params = split (eventContent, ",");
          if (params != NULL)
            {
              if (params->size () == 3)
                {
                  string strCode, strX, strY;
                  strCode = (*params)[0];
                  eventCode = CodeMap::getInstance ()->getCode (strCode);
                  if (eventCode == CodeMap::KEY_TAP)
                    {
                      strX = (*params)[1];
                      strY = (*params)[2];

                      tapObject (DeviceDomain::CT_PASSIVE, xstrto_int (strX), xstrto_int (strY));
                    }
                  else if (eventCode != CodeMap::KEY_NULL)
                    {
                      im->postInputEvent (eventCode);
                    }
                }
              delete params;
            }
        }
      else
        {
          eventCode = CodeMap::getInstance ()->getCode (eventContent);
          if (eventCode != CodeMap::KEY_NULL)
            {
              im->postInputEvent (eventCode);
            }
        }
    }

  return true;
}

void
FormatterMultiDevice::addActiveUris (string baseUri, vector<string> *uris)
{
  if (activeUris != NULL)
    {
      delete activeUris;
    }

  activeUris = uris;
  activeBaseUri = baseUri;
  clog << "FormatterMultiDevice::addActiveUris activeBaseUri=" << baseUri
       << endl;
}

void
FormatterMultiDevice::updatePassiveDevices ()
{
  postMediaContent (DeviceDomain::CT_PASSIVE);
}

void
FormatterMultiDevice::updateStatus (short code, arg_unused (string parameter),
                                    short type, arg_unused (string value))
{
  switch (code)
    {
    case IPlayer::PL_NOTIFY_UPDATECONTENT:
      if (type == IPlayer::TYPE_PASSIVEDEVICE)
        {
          FormatterMultiDevice::updatePassiveDevices ();
        }
      break;

    default:
      break;
    }
}

GINGA_FORMATTER_END