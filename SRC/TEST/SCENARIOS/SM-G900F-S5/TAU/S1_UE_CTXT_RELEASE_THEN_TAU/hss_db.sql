-- MySQL dump 10.13  Distrib 5.5.47, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: oai_db
-- ------------------------------------------------------
-- Server version	5.5.47-0ubuntu0.14.04.1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `apn`
--

DROP TABLE IF EXISTS `apn`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `apn` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `apn-name` varchar(60) NOT NULL,
  `pdn-type` enum('IPv4','IPv6','IPv4v6','IPv4_or_IPv6') NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `apn-name` (`apn-name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `apn`
--

LOCK TABLES `apn` WRITE;
/*!40000 ALTER TABLE `apn` DISABLE KEYS */;
/*!40000 ALTER TABLE `apn` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mmeidentity`
--

DROP TABLE IF EXISTS `mmeidentity`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mmeidentity` (
  `idmmeidentity` int(11) NOT NULL AUTO_INCREMENT,
  `mmehost` varchar(255) DEFAULT NULL,
  `mmerealm` varchar(200) DEFAULT NULL,
  `UE-Reachability` tinyint(1) NOT NULL COMMENT 'Indicates whether the MME supports UE Reachability Notifcation',
  PRIMARY KEY (`idmmeidentity`)
) ENGINE=MyISAM AUTO_INCREMENT=43 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mmeidentity`
--

LOCK TABLES `mmeidentity` WRITE;
/*!40000 ALTER TABLE `mmeidentity` DISABLE KEYS */;
INSERT INTO `mmeidentity` VALUES (2,'yang.openair4G.eur','openair4G.eur',0),(1,'ng40-erc.openair4G.eur','openair4G.eur',0),(3,'abeille.openair4G.eur','openair4G.eur',0);
/*!40000 ALTER TABLE `mmeidentity` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pdn`
--

DROP TABLE IF EXISTS `pdn`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pdn` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `apn` varchar(60) NOT NULL,
  `pdn_type` enum('IPv4','IPv6','IPv4v6','IPv4_or_IPv6') NOT NULL DEFAULT 'IPv4',
  `pdn_ipv4` varchar(15) DEFAULT '0.0.0.0',
  `pdn_ipv6` varchar(45) CHARACTER SET latin1 COLLATE latin1_general_ci DEFAULT '0:0:0:0:0:0:0:0',
  `aggregate_ambr_ul` int(10) unsigned DEFAULT '50000000',
  `aggregate_ambr_dl` int(10) unsigned DEFAULT '100000000',
  `pgw_id` int(11) NOT NULL,
  `users_imsi` varchar(15) NOT NULL,
  `qci` tinyint(3) unsigned NOT NULL DEFAULT '9',
  `priority_level` tinyint(3) unsigned NOT NULL DEFAULT '15',
  `pre_emp_cap` enum('ENABLED','DISABLED') DEFAULT 'DISABLED',
  `pre_emp_vul` enum('ENABLED','DISABLED') DEFAULT 'DISABLED',
  `LIPA-Permissions` enum('LIPA-prohibited','LIPA-only','LIPA-conditional') NOT NULL DEFAULT 'LIPA-only',
  PRIMARY KEY (`id`,`pgw_id`,`users_imsi`),
  KEY `fk_pdn_pgw1_idx` (`pgw_id`),
  KEY `fk_pdn_users1_idx` (`users_imsi`)
) ENGINE=MyISAM AUTO_INCREMENT=48 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pdn`
--

LOCK TABLES `pdn` WRITE;
/*!40000 ALTER TABLE `pdn` DISABLE KEYS */;
INSERT INTO `pdn` VALUES (1,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930000000001',9,15,'DISABLED','ENABLED','LIPA-only'),(41,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'20834123456789',9,15,'DISABLED','ENABLED','LIPA-only'),(40,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'20810000001234',9,15,'DISABLED','ENABLED','LIPA-only'),(42,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'31002890832150',9,15,'DISABLED','ENABLED','LIPA-only'),(16,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208950000000002',9,15,'DISABLED','ENABLED','LIPA-only'),(43,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'001010123456789',9,15,'DISABLED','ENABLED','LIPA-only'),(2,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930000000002',9,15,'DISABLED','ENABLED','LIPA-only'),(3,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930000000003',9,15,'DISABLED','ENABLED','LIPA-only'),(4,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930000000004',9,15,'DISABLED','ENABLED','LIPA-only'),(5,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930000000005',9,15,'DISABLED','ENABLED','LIPA-only'),(6,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930000000006',9,15,'DISABLED','ENABLED','LIPA-only'),(7,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930000000007',9,15,'DISABLED','ENABLED','LIPA-only'),(8,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208940000000001',9,15,'DISABLED','ENABLED','LIPA-only'),(9,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208940000000002',9,15,'DISABLED','ENABLED','LIPA-only'),(10,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208940000000003',9,15,'DISABLED','ENABLED','LIPA-only'),(11,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208940000000004',9,15,'DISABLED','ENABLED','LIPA-only'),(12,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208940000000005',9,15,'DISABLED','ENABLED','LIPA-only'),(13,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208940000000006',9,15,'DISABLED','ENABLED','LIPA-only'),(14,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208940000000007',9,15,'DISABLED','ENABLED','LIPA-only'),(15,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208950000000001',9,15,'DISABLED','ENABLED','LIPA-only'),(17,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208950000000003',9,15,'DISABLED','ENABLED','LIPA-only'),(18,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208950000000004',9,15,'DISABLED','ENABLED','LIPA-only'),(19,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208950000000005',9,15,'DISABLED','ENABLED','LIPA-only'),(20,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208950000000006',9,15,'DISABLED','ENABLED','LIPA-only'),(21,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208950000000007',9,15,'DISABLED','ENABLED','LIPA-only'),(22,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001100',9,15,'DISABLED','ENABLED','LIPA-only'),(23,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001101',9,15,'DISABLED','ENABLED','LIPA-only'),(24,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001102',9,15,'DISABLED','ENABLED','LIPA-only'),(25,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001103',9,15,'DISABLED','ENABLED','LIPA-only'),(26,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001104',9,15,'DISABLED','ENABLED','LIPA-only'),(27,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001105',9,15,'DISABLED','ENABLED','LIPA-only'),(28,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001106',9,15,'DISABLED','ENABLED','LIPA-only'),(29,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001107',9,15,'DISABLED','ENABLED','LIPA-only'),(30,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001108',9,15,'DISABLED','ENABLED','LIPA-only'),(31,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001109',9,15,'DISABLED','ENABLED','LIPA-only'),(32,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208920100001110',9,15,'DISABLED','ENABLED','LIPA-only'),(33,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930100001111',9,15,'DISABLED','ENABLED','LIPA-only'),(34,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930100001112',9,15,'DISABLED','ENABLED','LIPA-only'),(35,'oai.ipv4','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930100001113',9,15,'DISABLED','ENABLED','LIPA-only'),(44,'operator','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930100001113',9,15,'DISABLED','ENABLED','LIPA-only'),(45,'operator','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930100001112',9,15,'DISABLED','ENABLED','LIPA-only'),(46,'operator','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208930100001111',9,15,'DISABLED','ENABLED','LIPA-only'),(47,'operator','IPv4','0.0.0.0','0:0:0:0:0:0:0:0',50000000,100000000,3,'208950000000002',9,15,'DISABLED','ENABLED','LIPA-only');
/*!40000 ALTER TABLE `pdn` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pgw`
--

DROP TABLE IF EXISTS `pgw`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pgw` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `ipv4` varchar(15) NOT NULL,
  `ipv6` varchar(39) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `ipv4` (`ipv4`),
  UNIQUE KEY `ipv6` (`ipv6`)
) ENGINE=MyISAM AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pgw`
--

LOCK TABLES `pgw` WRITE;
/*!40000 ALTER TABLE `pgw` DISABLE KEYS */;
INSERT INTO `pgw` VALUES (1,'127.0.0.1','0:0:0:0:0:0:0:1'),(2,'192.168.56.101',''),(3,'10.0.0.2','0');
/*!40000 ALTER TABLE `pgw` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `terminal-info`
--

DROP TABLE IF EXISTS `terminal-info`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `terminal-info` (
  `imei` varchar(15) NOT NULL,
  `sv` varchar(2) NOT NULL,
  UNIQUE KEY `imei` (`imei`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `terminal-info`
--

LOCK TABLES `terminal-info` WRITE;
/*!40000 ALTER TABLE `terminal-info` DISABLE KEYS */;
/*!40000 ALTER TABLE `terminal-info` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `imsi` varchar(15) NOT NULL COMMENT 'IMSI is the main reference key.',
  `msisdn` varchar(46) DEFAULT NULL COMMENT 'The basic MSISDN of the UE (Presence of MSISDN is optional).',
  `imei` varchar(15) DEFAULT NULL COMMENT 'International Mobile Equipment Identity',
  `imei_sv` varchar(2) DEFAULT NULL COMMENT 'International Mobile Equipment Identity Software Version Number',
  `ms_ps_status` enum('PURGED','NOT_PURGED') DEFAULT 'PURGED' COMMENT 'Indicates that ESM and EMM status are purged from MME',
  `rau_tau_timer` int(10) unsigned DEFAULT '120',
  `ue_ambr_ul` bigint(20) unsigned DEFAULT '50000000' COMMENT 'The Maximum Aggregated uplink MBRs to be shared across all Non-GBR bearers according to the subscription of the user.',
  `ue_ambr_dl` bigint(20) unsigned DEFAULT '100000000' COMMENT 'The Maximum Aggregated downlink MBRs to be shared across all Non-GBR bearers according to the subscription of the user.',
  `access_restriction` int(10) unsigned DEFAULT '60' COMMENT 'Indicates the access restriction subscription information. 3GPP TS.29272 #7.3.31',
  `mme_cap` int(10) unsigned zerofill DEFAULT NULL COMMENT 'Indicates the capabilities of the MME with respect to core functionality e.g. regional access restrictions.',
  `mmeidentity_idmmeidentity` int(11) NOT NULL DEFAULT '0',
  `key` varbinary(16) NOT NULL DEFAULT '0' COMMENT 'UE security key',
  `RFSP-Index` smallint(5) unsigned NOT NULL DEFAULT '1' COMMENT 'An index to specific RRM configuration in the E-UTRAN. Possible values from 1 to 256',
  `urrp_mme` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'UE Reachability Request Parameter indicating that UE activity notification from MME has been requested by the HSS.',
  `sqn` bigint(20) unsigned zerofill NOT NULL,
  `rand` varbinary(16) NOT NULL,
  `OPc` varbinary(16) DEFAULT NULL COMMENT 'Can be computed by HSS',
  PRIMARY KEY (`imsi`,`mmeidentity_idmmeidentity`),
  KEY `fk_users_mmeidentity_idx1` (`mmeidentity_idmmeidentity`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES ('20834123456789','380561234567','35609204079300',NULL,'PURGED',50,40000000,100000000,47,0000000000,2,'+�E��ų\0�,IH��H',0,0,00000000000000000096,'Px�X \Z1��x��','g퐐jS+Aq���6Y'),('20810000001234','33611123456','35609204079299',NULL,'PURGED',120,40000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000281454575616225,'\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0','�\'��i.u2fz;`]'),('31002890832150','33638060059','35611302209414',NULL,'PURGED',120,40000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000012416,'`�F�݆��D��ϛ���','�\'��i.u2fz;`]'),('001010123456789','33600101789','35609204079298',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'\0	\n\r',1,0,00000000000000000351,'\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0','$�_|/+6��%/%���'),('208930000000001','33638030001','35609204079301',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208950000000002','33638050002','35609204079502',NULL,'NOT_PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000020471,'\0	\n\r','�\'��i.u2fz;`]'),('208950000000003','33638050003','35609204079503',NULL,'NOT_PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000012311,'\0	\n\r','�\'��i.u2fz;`]'),('208950000000004','33638050004','35609204079504',NULL,'PURGED',120,50000000,100000000,47,0000000000,3,'��G?/�Д����	|hb',1,0,00000000000000012215,'56f0261d9d051063','�\'��i.u2fz;`]'),('208950000000005','33638050005','35609204079505',NULL,'PURGED',120,50000000,100000000,47,0000000000,3,'��G?/�Д����	|hb',1,0,00000000000000012215,'56f0261d9d051063','�\'��i.u2fz;`]'),('208950000000001','33638050001','35609204079501',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208950000000006','33638050006','35609204079506',NULL,'PURGED',120,50000000,100000000,47,0000000000,3,'��G?/�Д����	|hb',1,0,00000000000000012215,'56f0261d9d051063','�\'��i.u2fz;`]'),('208950000000007','33638050007','35609204079507',NULL,'PURGED',120,50000000,100000000,47,0000000000,3,'��G?/�Д����	|hb',1,0,00000000000000012215,'56f0261d9d051063','�\'��i.u2fz;`]'),('208930000000002','33638030002','35609204079302',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208930000000003','33638030003','35609204079303',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208930000000004','33638030004','35609204079304',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208930000000005','33638030005','35609204079305',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208930000000006','33638030006','35609204079306',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208930000000007','33638030007','35609204079307',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208940000000007','33638040007','35609204079407',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208940000000006','33638040006','35609204079406',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208940000000005','33638040005','35609204079405',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208940000000004','33638040004','35609204079404',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208940000000003','33638040003','35609204079403',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208940000000002','33638040002','35609204079402',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208940000000001','33638040001','35609204079401',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'��wq��gzW�Ё��Z]','�\'��i.u2fz;`]'),('208920100001100','33638020001','35609204079201',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208920100001101','33638020001','35609204079201',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006167,'Y�P:(xnj 4��rB','�\'��i.u2fz;`]'),('208920100001102','33638020002','35609204079202',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208920100001103','33638020003','35609204079203',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208920100001104','33638020004','35609204079204',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208920100001105','33638020005','35609204079205',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208920100001106','33638020006','35609204079206',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208920100001107','33638020007','35609204079207',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208920100001108','33638020008','35609204079208',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208920100001109','33638020009','35609204079209',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208920100001110','33638020010','35609204079210',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208930100001111','33638030011','35609304079211',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208930100001112','33638030012','35609304079212',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006103,'ebd07771ace8677a','�\'��i.u2fz;`]'),('208930100001113','33638030013','35609304079213',NULL,'PURGED',120,50000000,100000000,47,0000000000,2,'��G?/�Д����	|hb',1,0,00000000000000006263,'�SNܒ�Iv��e�6','�\'��i.u2fz;`]');
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-02-11 16:00:07
