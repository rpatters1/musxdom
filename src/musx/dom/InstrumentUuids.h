/*
 * Copyright (C) 2025, Robert Patterson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <string_view>

namespace musx {
namespace uuid {

inline constexpr std::string_view BlankStaff             = "54422b22-4627-4100-abbf-064eedc15fe3";
inline constexpr std::string_view GrandStaff             = "24b4c18d-6f6a-461c-983f-e0b0c3abcdbc";
inline constexpr std::string_view Unknown                = "a925648a-abc9-4dc7-a619-a6ce355ad33c";

// Strings
inline constexpr std::string_view Violin                 = "29d81c10-dfed-42e4-8d93-64af7d6d5689";
inline constexpr std::string_view Viola                  = "992dcb2b-64a2-454b-b043-2a530d24fc18";
inline constexpr std::string_view Cello                  = "33894688-c326-4f8f-b4e2-7a08745e7bcc";
inline constexpr std::string_view DoubleBass             = "4924f736-97a2-443d-b386-03de2ae6a848";
inline constexpr std::string_view ViolinSection          = "5bab3582-2dbd-4864-baa9-b022efd075ad";
inline constexpr std::string_view ViolaSection           = "5a55eca7-40f5-454b-b920-c8879fa03bcc";
inline constexpr std::string_view CelloSection           = "255c105e-78a1-4c78-8755-8febc86d0f43";
inline constexpr std::string_view VioloncelloSection     = "df0e2c08-adcf-4095-a3c1-dd355216bedd";
inline constexpr std::string_view DoubleBassSection      = "08a26335-79bc-4d10-a879-7f9d752c199a";
inline constexpr std::string_view ContrabassSection      = "639064b2-b067-4c4a-a114-392c1ef6ebd8";
inline constexpr std::string_view StringEnsemble         = "d73cf404-a025-4395-a7d1-10d10e8e3b4c";
inline constexpr std::string_view ViolaDAmore            = "7e264095-22f9-4cea-98ce-cfde9f1588b6";
inline constexpr std::string_view Ajaeng                 = "4da5baf0-a6c8-4993-83d7-882bbb2adbb9";
inline constexpr std::string_view Arpeggione             = "cec73493-406a-4100-9d9f-7706940a39f0";
inline constexpr std::string_view Baryton                = "73eff966-03ff-4c38-b71a-f48fdfe47312";
inline constexpr std::string_view ByzantineLyra          = "f240ab3f-8799-4834-a265-2f6b6abc8f4c";
inline constexpr std::string_view CretanLyra             = "b0c43bb0-eae5-410f-ba40-cf33d63994f1";
inline constexpr std::string_view Crwth                  = "e40c329a-cc79-4477-a314-0786731390b3";
inline constexpr std::string_view Dahu                   = "562a0d3f-0073-44e5-a4f1-430935a72f7e";
inline constexpr std::string_view Dangao                 = "d07f465c-0d1a-4eef-884e-feb414088078";
inline constexpr std::string_view Dihu                   = "9102cdc2-13f9-4818-ba25-34226bd2d893";
inline constexpr std::string_view Erhu                   = "df24de8e-08c8-4c1b-8f20-8673ce15009d";
inline constexpr std::string_view Erxian                 = "8fd59c9c-f49d-4f05-b8db-dba34d1bf462";
inline constexpr std::string_view Fiddle                 = "00fb6661-7b4a-411e-8ec6-7f6c15d9f3b3";
inline constexpr std::string_view Gaohu                  = "06d74eb0-5dd8-46ac-b1f9-dfdb5e1b1857";
inline constexpr std::string_view Gehu                   = "ced43fa4-9b0c-4413-be51-290a8e7b70d6";
inline constexpr std::string_view Haegeum                = "2c6d5aaa-04f6-4536-9564-40eb62ef9f1e";
inline constexpr std::string_view HardangerFiddle        = "c0666be0-a127-465c-9608-76cbdeb5e902";
inline constexpr std::string_view HurdyGurdy             = "06a9413e-9739-4635-b968-bafedcb11db6";
inline constexpr std::string_view Igil                   = "99b31179-d930-48ef-b59b-de49a5630156";
inline constexpr std::string_view Kamancha               = "50d11032-113c-4799-a473-e40db1161e8f";
inline constexpr std::string_view Kokyu                  = "89102882-aa4f-46aa-abed-255d279b679e";
inline constexpr std::string_view Kora                   = "78e799d1-9c1f-42fc-86d3-0a518050ec81";
inline constexpr std::string_view LaruAn                 = "517fac0f-760c-4cdb-aae8-8a29625db520";
inline constexpr std::string_view Leiqin                 = "eacec010-a733-439e-ab56-5ed48f16aa31";
inline constexpr std::string_view Lirone                 = "1092088e-ba6b-4147-883d-a39b2f4b0d9a";
inline constexpr std::string_view MorinKhuur             = "32ec0199-96ef-49a5-ad89-fb180f7a3a2a";
inline constexpr std::string_view Nyckelharpa            = "77bc62ef-8724-4ffd-83af-4fcfcbb65b12";
inline constexpr std::string_view Octobass               = "22b93602-f5a2-4aa3-a581-99dc3ac3aac6";
inline constexpr std::string_view Rebab                  = "ba835d76-ee03-4d68-92cf-37c47aa774e4";
inline constexpr std::string_view Rebec                  = "252f3cc6-4cc4-4b31-ba9d-4b0f1a7a729a";
inline constexpr std::string_view Sarangi                = "37021f3f-2acd-4c00-b2e0-71bb0f07ae7e";
inline constexpr std::string_view SarangiDrone           = "341ba385-fda1-473b-afa9-8bfd0d576f7f";
inline constexpr std::string_view StrohViolin            = "013f1005-058e-4270-97b6-89fe25d11979";
inline constexpr std::string_view Trombamarina           = "71822c45-e84e-44ce-8031-46092e74dc5e";
inline constexpr std::string_view Vielle                 = "7b07fc24-0b7a-484a-a1bf-abaab3d97411";
inline constexpr std::string_view Viol                   = "6fed40b7-1524-429b-8560-bb3f08209a99";
inline constexpr std::string_view ViolaDaGamba           = "37fad7f4-b52f-4af5-b75a-ab214d9e3a5a";
inline constexpr std::string_view ViolinoPiccolo         = "ae4668c7-0766-4dfd-92c3-cd16a487ff83";
inline constexpr std::string_view VioloncelloPiccolo     = "121fb3dd-37d9-4fd0-bbaf-8074ba143f03";
inline constexpr std::string_view Violotta               = "baad2449-f875-4360-9585-b7f3d8ceed1c";
inline constexpr std::string_view Zhonghu                = "08e9eb3f-42d8-4e64-8b51-1b30afcb76dc";

// Keyboards
inline constexpr std::string_view Piano = "776a2734-3f38-4cac-8fa2-90e62b368ec1";
inline constexpr std::string_view PianoNoName       = "38cd3cd9-3160-4baa-9bfd-c2d9f353dafd";
inline constexpr std::string_view Harpsichord       = "5f34a4c0-b181-4d6d-8475-725374517ac5";
inline constexpr std::string_view Organ             = "723972d2-a005-492a-b92c-2cb1a6b3f21d";
inline constexpr std::string_view Organ2Staff       = "ab39d5e6-a742-4af3-a0a6-269422e21ec3";
inline constexpr std::string_view Celesta           = "2827e1dc-72a3-4228-87fb-c0daee5ca2ad";
inline constexpr std::string_view Accordion         = "58734c17-246e-4e83-8975-9a0d55577d16";
inline constexpr std::string_view Melodica          = "a41f4a27-6e41-47f1-973f-2c7fc66b3e68";
inline constexpr std::string_view ElectricPiano     = "4b54f9c4-35e3-41f8-9a17-a8797df0f057";
inline constexpr std::string_view Clavinet          = "ba97d794-3171-49e6-ac1b-af4b29ee61b8";
inline constexpr std::string_view SynthPad          = "962083d1-baf2-4904-a6f2-5c7e8f91de66";
inline constexpr std::string_view SynthLead         = "de90a22f-c65d-4e20-8aff-9f4bcbf2ae4f";
inline constexpr std::string_view SynthBrass        = "e15cdc19-cb49-4f00-8ee4-7555af7c7759";
inline constexpr std::string_view SynthSoundtrack   = "55248216-3918-474d-b5e4-3349f253392d";
inline constexpr std::string_view SoundFX           = "654cb42c-6127-4c60-bdb9-6b2accf66f22";
inline constexpr std::string_view Harmonium         = "f3caf816-7be6-48e0-8067-07202cdb9b49";
inline constexpr std::string_view OndesMartenot     = "8fae711b-6eb2-4e99-9216-9248a25a4d3c";
inline constexpr std::string_view Theremin          = "39515883-5827-44f6-bc14-99e5a4976237";
inline constexpr std::string_view Virginal          = "88eb2453-6649-4f3f-b98c-af2226bb4099";
inline constexpr std::string_view Clavichord        = "9322d689-68bc-4699-b77e-d370340830d7";

// Voices
inline constexpr std::string_view SopranoVoice          = "b35a14d8-c981-4b8a-a7e4-9a2f8f6be99a";
inline constexpr std::string_view AltoVoice             = "ab3fc62c-9de9-4018-adf7-58a9d4441a1a";
inline constexpr std::string_view TenorVoice            = "03844f81-9f10-431d-813c-659d7533b794";
inline constexpr std::string_view BaritoneVoice         = "6637f6a3-8f69-4dbf-9654-2287933ee7d3";
inline constexpr std::string_view BassVoice             = "e9a8eb68-25a0-4c9b-81a1-6e0517ca15be";
inline constexpr std::string_view Vocals                = "a253b03e-dc1e-43e3-a252-534c618fe473";
inline constexpr std::string_view Voice                 = "3a3622cc-fc9a-42e0-a743-6b200dd8ce88";
inline constexpr std::string_view VoiceNoName           = "07639481-d9a4-4c3a-b754-f454002bb742";
inline constexpr std::string_view MezzoSopranoVoice     = "9b62dc60-4b05-41e1-9b4d-d8b7d19553d0";
inline constexpr std::string_view ContraltoVoice        = "aba0e110-478a-4c69-b8be-908c280d64b2";
inline constexpr std::string_view CountertenorVoice     = "e61680e6-4a19-48bd-a0d7-f0f1373c8be2";
inline constexpr std::string_view BassBaritoneVoice     = "e453be0a-d0fc-44b3-8b0a-5c4a95157d44";
inline constexpr std::string_view ChoirAahs             = "cc4e753f-85b9-43c6-b462-a5ec36a01167";
inline constexpr std::string_view ChoirOohs             = "28426f8d-3f42-448b-913d-3ee37dc1c871";
inline constexpr std::string_view Yodel                 = "d7373ec2-d4ad-4ce8-84e7-ae9c5f216db6";
inline constexpr std::string_view Beatbox               = "ac62ce27-0e7a-4f23-849c-fe3dee395910";
inline constexpr std::string_view Kazoo                 = "c79f9662-44d7-442f-b5e6-a8b0085e92f4";
inline constexpr std::string_view Talkbox               = "465cb9eb-e1a3-4573-8572-7cc1096075db";
inline constexpr std::string_view VocalPercussion       = "c4843056-dd9e-45b2-a3af-eca60fdb97ab";

// Woodwinds
inline constexpr std::string_view Piccolo                = "7423e6af-ab6f-41a4-832f-ffc6453c6bb5";
inline constexpr std::string_view Flute                  = "2575136d-9927-47bd-a727-f554c169257e";
inline constexpr std::string_view AltoFlute              = "71d2b8ec-becd-42ef-b99f-ee9264db925d";
inline constexpr std::string_view Oboe                   = "172a455a-d1b9-4a03-aa80-c863d587f209";
inline constexpr std::string_view OboeDAmore             = "b70397fa-18ef-4841-8ccb-d5a9602e457d";
inline constexpr std::string_view EnglishHorn            = "e4fc7b0a-5cd3-445b-ae1f-776764513741";
inline constexpr std::string_view ClarinetBFlat          = "a16af0c1-8df8-476d-8d74-0d6e1972846b";
inline constexpr std::string_view ClarinetA              = "9d3e28eb-0640-41f7-9144-0fdef3046d7c";
inline constexpr std::string_view ClarinetEFlat          = "7e83467b-162e-4f30-b4a6-18a6d8beb202";
inline constexpr std::string_view AltoClarinet           = "6c186f89-5f5d-45ea-ab13-26e4bf267d80";
inline constexpr std::string_view ContraltoClarinet      = "5a1020cd-f0ce-45e8-bc99-49fbf5f6890f";
inline constexpr std::string_view BassClarinet           = "40d3efff-30c3-4fd7-b830-03907360feed";
inline constexpr std::string_view ContrabassClarinet     = "7724f2be-b200-4404-bc08-4ef719a1373c";
inline constexpr std::string_view Bassoon                = "ec6fcc9b-d539-4a4a-a677-92a37298fec1";
inline constexpr std::string_view Contrabassoon          = "0d158020-9bf8-4d56-b62c-a6f0bf5f4ccf";
inline constexpr std::string_view WindSection            = "0a798583-8a07-42c3-a6c1-c0e6c5cc622b";
inline constexpr std::string_view SopranoSax             = "ef0f2594-d57b-4cac-b351-f2f906538424";
inline constexpr std::string_view AltoSax                = "4cb4cfa1-0b19-4053-8bf5-6b2cb0a2363b";
inline constexpr std::string_view TenorSax               = "7093ddd4-385c-481d-ab3d-39f03bc03b19";
inline constexpr std::string_view BaritoneSax            = "8f767980-0718-4154-a312-20f458ee2096";
inline constexpr std::string_view SopranoRecorder        = "0057fd7f-aba1-4149-97b5-98fe8ca28993";
inline constexpr std::string_view SopraninoRecorder      = "52559fd3-bb96-403f-97d3-b1f2c5c2f468";
inline constexpr std::string_view AltoRecorder           = "6b43616e-06ad-4397-9ca5-889dcbda20a9";
inline constexpr std::string_view TenorRecorder          = "7c64b00d-cbd1-45a4-9969-77335de7f99c";
inline constexpr std::string_view BassRecorder           = "ed544580-e477-410b-a54d-dbb14edbb31e";
inline constexpr std::string_view DescantRecorder        = "56470843-614b-4bbd-bb83-bf928bb10d7d";
inline constexpr std::string_view Ocarina                = "b0330079-36e9-4f3a-8b2e-e1e3432292fe";
inline constexpr std::string_view PennyWhistle           = "37bb50bc-04fb-494c-8d52-942149b0e757";
inline constexpr std::string_view PennyWhistleD          = "54b39327-75aa-417b-a4d3-70bd5be2be9c";
inline constexpr std::string_view PennyWhistleG          = "c94c25b2-83fa-44c3-8ba6-a218d6e12eca";
inline constexpr std::string_view LowIrishWhistle        = "dd31d2dc-cca6-489e-98ae-7447584b7ce9";
inline constexpr std::string_view TinWhistleBFlat        = "c2853206-ce69-4670-94e6-095278d7eca9";
inline constexpr std::string_view Harmonica              = "59b8b559-24c6-4ef0-8e67-8dcf4e1708fb";
inline constexpr std::string_view BassHarmonica          = "941ded93-3dc2-41cb-85d0-4c033988caef";
inline constexpr std::string_view Concertina             = "cc65d11c-9cfc-46ed-ba60-cf9ec7bee3d5";
inline constexpr std::string_view Bandoneon              = "3320f54a-d594-438c-b276-6ac67d3d26b0";
inline constexpr std::string_view HornF_WWQuintet        = "6cc5f057-3980-4562-b9a4-257403d17e96"; // Brass!
inline constexpr std::string_view Bagpipes               = "85399b62-95db-463e-abaf-da5f8405d57f";
inline constexpr std::string_view UilleannPipes          = "62a9c9ce-4379-4a58-9f80-2818b66774b4";
inline constexpr std::string_view GaidaPipes             = "0bef0abc-85a3-4f5f-940a-3a9cfc7c128b";
inline constexpr std::string_view ContraAltoFlute        = "9efb024e-88cb-4d91-97c8-1b84bc85185e";
inline constexpr std::string_view BassFlute              = "b15ed58a-8c7d-487b-b54a-049f52b0a264";
inline constexpr std::string_view ContrabassFlute        = "76326fd2-1a85-4620-9696-2b4b8d6e8f63";
inline constexpr std::string_view DoubleContrabassFlute  = "1b7dc77e-17ef-4a7f-80d7-01b8eb47aef5";
inline constexpr std::string_view HyperbassFlute         = "4413d227-8660-45ff-beb1-584161ccbf7e";
inline constexpr std::string_view PanPipes               = "a8bd8c3b-577d-4812-b500-f76b3cf9ab2b";
inline constexpr std::string_view Fife                   = "54ebe5da-7965-4829-b919-b21841ffd05a";
inline constexpr std::string_view BottleBlow             = "9fc11d96-9d40-4c07-8135-d6178c1548fe";
inline constexpr std::string_view Jug                    = "1799027e-dfff-49f9-a637-03b3db915e9f";
inline constexpr std::string_view PiccoloOboe            = "f5424bcc-295a-4763-9c4c-52cbaa5356ea";
inline constexpr std::string_view PiccoloHeckelphone     = "9b97fd4a-5e7d-45ec-a85f-00304efa6fd3";
inline constexpr std::string_view Heckelphone            = "944e0d72-f13a-43e3-985d-272b12776c8a";
inline constexpr std::string_view BassOboe               = "99f375d2-da09-4eac-aa7c-11bc0df567f3";
inline constexpr std::string_view BassetClarinet         = "b74f3793-ed8e-4b0b-93ab-864097d4658b";
inline constexpr std::string_view BassetHorn             = "5a02ed14-9fbb-41b6-b0b2-1ae2052d28ca";
inline constexpr std::string_view Hornpipe               = "e6449624-6912-454b-8f16-a8d44692ac8d";
inline constexpr std::string_view PiccoloClarinet        = "00b0b3a3-8906-46a1-9bc5-c5801f307a73";
inline constexpr std::string_view Saxonette              = "ee4bfc3d-e50a-41c4-984a-0b5c6a002cb8";
inline constexpr std::string_view SopraninoSax           = "1ac144c7-73e3-4c96-9c3d-4f8a7324f2a0";
inline constexpr std::string_view MezzoSopranoSax        = "aefe6f0b-3ccc-41f8-b421-d94c9e7a167d";
inline constexpr std::string_view Sopranino              = "977e30e3-9333-4973-8342-24190390238d"; // Assuming Sopranino = SoprilLo?
inline constexpr std::string_view CMelodySax             = "04d3f8ec-bb16-431f-b609-580d2d2d1169";
inline constexpr std::string_view Aulochrome             = "e09e3499-e833-48a5-a2ba-b86e01c6dfe9";
inline constexpr std::string_view Xaphoon                = "5be7a8a7-99e6-45a7-b625-3561788e585e";
inline constexpr std::string_view BassSax                = "1e325f9e-bd57-49ca-9c94-e53a51f6108d";
inline constexpr std::string_view ContrabassSax          = "5f4bbe18-a82f-4435-80ff-2f39f4492f3e";
inline constexpr std::string_view SubContrabassSax       = "dfd0e0dc-c1b2-4f6a-b85f-41a1c59b3976";
inline constexpr std::string_view Tubax                  = "99c073a3-70b4-4418-9380-09f2f550e15c";
inline constexpr std::string_view Bansuri                = "f26581a2-9cad-4523-b472-95d058b1a82e";
inline constexpr std::string_view Danso                  = "63208df8-8e11-4e19-b7b3-27c097e861f5";
inline constexpr std::string_view Dizi                   = "442552fd-b7f8-488b-880a-f141f617cf90";
inline constexpr std::string_view DilliKaval             = "abfc63a8-9453-4958-a3fb-60e2fc05006f";
inline constexpr std::string_view Diple                  = "5a6e6ce3-155a-46cd-8c15-177e2e46ec3d";
inline constexpr std::string_view DoubleFlute            = "bf8d7ea1-c1fa-44b7-adc3-6d7006dd7637";
inline constexpr std::string_view Dvojnice               = "867c9afa-428f-4918-99fb-a580c3941dd1";
inline constexpr std::string_view DvojniceDrone          = "73019a0e-2f69-4b89-947e-cadbc91340ad";
inline constexpr std::string_view Flageolet              = "8070a42b-09f7-4142-bceb-5d48f0bccf9a";
inline constexpr std::string_view Fujara                 = "dbebe448-5f02-4cbe-b2af-8037506a0928";
inline constexpr std::string_view Gemshorn               = "4c4bf6bd-b778-48ee-9212-d463522c0213";
inline constexpr std::string_view Hocchiku               = "4a12d25d-5726-4886-8e98-991d5625b317";
inline constexpr std::string_view Hun                    = "1fd77046-302d-49fe-8342-d869272c148c";
inline constexpr std::string_view IrishFlute             = "badaf8de-498d-4629-b853-97fdb849c83d";
inline constexpr std::string_view Kaval                  = "7792014c-81e2-4ff5-b45a-ca00f6d5a137";
inline constexpr std::string_view Khlui                  = "cdf417bc-db53-4488-8941-14bc8044f2f7";
inline constexpr std::string_view KnotweedFlute          = "3387f5dc-573e-4d38-b6aa-d8892360ef8a";
inline constexpr std::string_view KoncovkaAltoFlute      = "3e47d131-819b-4d32-ae83-95ef468e65fd";
inline constexpr std::string_view Koudi                  = "280914ad-66c8-4f22-86e1-f471a13ac008";
inline constexpr std::string_view Ney                    = "a21f447b-7fa8-49de-8c0e-3b9954ad986a";
inline constexpr std::string_view Nohkan                 = "8708a296-2db3-452a-b654-daadc164d0a9";
inline constexpr std::string_view NoseFlute              = "3dcf456a-b87a-46fb-ba6c-90d3a11c0e22";
inline constexpr std::string_view Palendag               = "6fd5d411-2f73-427b-a278-a129381c6a1d";
inline constexpr std::string_view Quena                  = "f92e2b02-3e6f-4267-9242-415cf95a9cf8";
inline constexpr std::string_view Ryuteki                = "52e2cabb-0314-402e-8bbe-997e98a65f4b";
inline constexpr std::string_view Shakuhachi             = "9259887f-83a7-4e91-b0cb-03d3bed5886c";
inline constexpr std::string_view ShepherdsPipe          = "4c8ad2dd-c863-4272-a317-6e33194969db";
inline constexpr std::string_view Shinobue               = "0dc9af9e-4a7a-44d9-9ede-a092a3e0fdf6";
inline constexpr std::string_view ShivaWhistle           = "0a0c1873-83fe-4bc7-bdb8-5e68e6bd09db";
inline constexpr std::string_view Shvi                   = "a788598c-02a8-42ff-a1c5-f575d5c8610c";
inline constexpr std::string_view Suling                 = "33b4ae63-2222-4d2d-b37c-b138bf383428";
inline constexpr std::string_view Tarka                  = "c19c42f1-42cf-417c-a362-77b3f4522d44";
inline constexpr std::string_view TenorOvertoneFlute     = "7ec8bc7c-76de-4473-946a-336df9312f68";
inline constexpr std::string_view Tumpong                = "f3050b4d-1a0e-4d00-9e34-3caedbb3ac5b";
inline constexpr std::string_view Venu                   = "91139f31-7457-4d12-b5d2-b7742178f4c2";
inline constexpr std::string_view Xiao                   = "dcb76d8d-16bc-4034-9571-27aa999634c3";
inline constexpr std::string_view Xun                    = "6bd9746f-aec7-4e09-bcf0-6b85ebac1ddd";
inline constexpr std::string_view Albogue                = "cc046dd9-f5b7-4e0e-aacc-349e38da8ff3";
inline constexpr std::string_view Alboka                 = "068a2f5d-dd22-47e8-ad53-b4b4dbd149ee";
inline constexpr std::string_view AltoCrumhorn           = "43a2bdba-df71-4768-ae08-955c380f4adb";
inline constexpr std::string_view Arghul                 = "f79879d9-f370-4acb-a1b5-0ce2dc6c3895";
inline constexpr std::string_view Bawu                   = "0c0a399c-6e82-4fb7-8f5a-c7fe3d12dedb";
inline constexpr std::string_view Chalumeau              = "2b0e17ba-f185-4b97-b094-71a4668edfe2";
inline constexpr std::string_view ClarinetteDAmour       = "58054d04-a50d-4c3b-9b99-abbb13c28850";
inline constexpr std::string_view Cornamuse              = "b68788f5-aed1-4d72-b4d4-facd99f28e90";
inline constexpr std::string_view Diplica                = "eb27b9dd-b27c-47bb-b8c8-a0284535bece";
inline constexpr std::string_view DoubleClarinet         = "5ec4aaff-7e36-4bef-bbaa-1a996ad42420";
inline constexpr std::string_view HeckelClarina          = "38f7cb85-166d-40fc-a71b-810564406fdc";
inline constexpr std::string_view HeckelphoneClarinet    = "cba3d8dd-e7a9-4b5e-bb6b-0e4ddc20fe63";
inline constexpr std::string_view Hirtenschalmei         = "81777381-f632-4f78-aa8b-bfd7b4fef7fa";
inline constexpr std::string_view Launeddas              = "2f5cac27-b724-466e-904e-af3ecd5e64d6";
inline constexpr std::string_view Maqrunah               = "a6140a3d-2ccb-494c-b96d-005d6abc2a6b";
inline constexpr std::string_view Mijwiz                 = "55b4d551-ed21-4705-af1a-dcd986fd08ef";
inline constexpr std::string_view Octavin                = "ee6e58a7-20f5-474e-b541-9fb456de6904";
inline constexpr std::string_view Pibgorn                = "079610c3-1203-4dff-b231-af31f1fce20c";
inline constexpr std::string_view Rauschpfeife           = "f454c420-f6af-45e7-bb73-5e4031b27918";
inline constexpr std::string_view Sipsi                  = "b379e5e9-44d5-4ca3-b039-3feea42ccd55";
inline constexpr std::string_view ModernTarogato         = "0545a1b4-452a-40dc-a4df-3e4746966614";
inline constexpr std::string_view TenorCrumhorn          = "48fa8484-5bc0-4d60-b632-c11c668f5143";
inline constexpr std::string_view Zhaleika               = "101e2231-696e-49fb-955d-500b3ce3ac31";
inline constexpr std::string_view Algaita                = "d841d923-42f6-4edb-8b6e-8842edb170b9";
inline constexpr std::string_view Bifora                 = "7ccb817f-82e7-4b52-88b1-2f097de86773";
inline constexpr std::string_view Bombarde               = "103e61ac-6b51-4a70-92ef-3fc3ae6b170c";
inline constexpr std::string_view Cromorne               = "07786b62-f42f-4db3-90f1-35e6e12a9f8f";
inline constexpr std::string_view Duduk                  = "9525113b-19b7-4ca6-a846-b417bc44bfcf";
inline constexpr std::string_view Dulcian                = "43620b28-a8bd-478e-a610-85af5c9a0480";
inline constexpr std::string_view Dulzaina               = "a7a2cb49-c0b2-4bc2-b733-af19bc46076a";
inline constexpr std::string_view Guan                   = "5466f457-7976-457f-a993-a5827497f54c";
inline constexpr std::string_view Guanzi                 = "ab97d8ca-ebcb-4ebe-b107-d1924a89ea74";
inline constexpr std::string_view Hichiriki              = "c372616c-4d6c-4f25-a930-d19204582be0";
inline constexpr std::string_view Hne                    = "ee53523b-5209-466d-b10e-c5f65ee442e7";
inline constexpr std::string_view JogiBaja               = "5ac64ac7-54ce-441b-8339-33af4f695113";
inline constexpr std::string_view KenBau                 = "1d80222d-bcc3-4605-9979-729847c83b51";
inline constexpr std::string_view Mizmar                 = "0f976aa0-7c54-45f8-a1a9-451125809ea8";
inline constexpr std::string_view Nadaswaram             = "8089cf04-f4a9-4c33-8327-3989757b825b";
inline constexpr std::string_view OboeDaCaccia           = "d228748f-7e5c-4646-9316-f7a2a85e4c31";
inline constexpr std::string_view Pi                     = "7471f961-590b-4382-b4a6-d05069cd19ae";
inline constexpr std::string_view Piri                   = "54ed0ce3-2d02-4aab-985e-8b1af576a294";
inline constexpr std::string_view PungiSnakeCharmer      = "25b468b4-ff45-4689-8774-ad13eb7b32c1";
inline constexpr std::string_view Rackett                = "ec8aad53-c0ae-471d-99f4-86eec9897f1b";
inline constexpr std::string_view ReedContrabass         = "28848103-882e-4f21-aa56-e99b3fc107d0";
inline constexpr std::string_view Rhaita                 = "c308dfcd-8e56-48a3-b008-d2b0cc30c161";
inline constexpr std::string_view Rothphone              = "01ba3b66-731b-4980-b994-89b481b5565b";
inline constexpr std::string_view Sarrusophone           = "8b6d9de3-7100-4926-8405-18d32a046fa8";
inline constexpr std::string_view Shawm                  = "654efca8-6eb3-460e-880b-c0193285887f";
inline constexpr std::string_view Shehnai                = "416debd0-939a-4241-8ce4-f1c9beaf7be6";
inline constexpr std::string_view Sopila                 = "3fbb3534-6714-4d41-8f65-1bec853cf688";
inline constexpr std::string_view Sorna                  = "e7d11adf-252f-4d95-a3ff-3fce49e798ba";
inline constexpr std::string_view Sralai                 = "ee80b114-9697-4edb-ba40-57930ec6ab03";
inline constexpr std::string_view Suona                  = "2e0cbdf7-bed5-4ac9-9d30-f7bc8044cdee";
inline constexpr std::string_view Surnay                 = "2d9c16cf-72c4-4a00-bb91-a6a8ab0b32a0";
inline constexpr std::string_view Taepyeongso            = "bdf82e47-6a76-4994-ac47-4f52f0111ddc";
inline constexpr std::string_view AncientTarogato        = "baff0f86-b7f2-466e-8d84-016a350f25af";
inline constexpr std::string_view TrompetaChina          = "cc2a08c1-7e44-4495-bc79-bdb963358bed";
inline constexpr std::string_view Zurla                  = "e24cf1f3-d12d-4e9b-919c-ea4d4fa4d836";
inline constexpr std::string_view Zurna                  = "b7cf7c1e-6b2f-4ca7-a188-b80b8380cd4a";
inline constexpr std::string_view KhaenMouthOrgan        = "642a83a4-4e29-4b84-8849-d4b6f9310455";
inline constexpr std::string_view Hulusi                 = "a040181f-4ded-4a8d-8725-70efd42767ce";
inline constexpr std::string_view Sheng                  = "d24f2747-9faf-416b-bf48-1f4143594ee1";

} // namespace uuid
} // namespace musx