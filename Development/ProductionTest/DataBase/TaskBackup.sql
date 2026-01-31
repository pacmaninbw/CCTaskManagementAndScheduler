-- phpMyAdmin SQL Dump
-- version 5.2.1deb3
-- https://www.phpmyadmin.net/
--
-- Host: localhost:3306
-- Generation Time: Jan 31, 2026 at 03:02 PM
-- Server version: 10.11.14-MariaDB-0ubuntu0.24.04.1
-- PHP Version: 8.3.6

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `PlannerTaskScheduleDB`
--
CREATE DATABASE IF NOT EXISTS `PlannerTaskScheduleDB` DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci;
USE `PlannerTaskScheduleDB`;

INSERT INTO `UserProfile` (`UserID`, `Organization_ID`, `LastName`, `FirstName`, `MiddleInitial`, `EmailAddress`, `LoginName`, `HashedPassWord`, `Preferences`, `UserAdded`, `LastLogin`) VALUES
(1, NULL, 'Chernick', 'Paul', 'A', 'paul.chernick@chernicksw.com', 'pacmaninbw', 'M1nus+1r3th', '8:30 AM;5:00 PM;1;1;1;0;', '2026-01-23 15:14:39', '2026-01-31 14:25:21');

--
-- Dumping data for table `Tasks`
--

INSERT INTO `Tasks` (`TaskID`, `CreatedBy`, `AsignedTo`, `Description`, `ParentTask`, `Status`, `PercentageComplete`, `CreatedOn`, `RequiredDelivery`, `ScheduledStart`, `ActualStart`, `EstimatedCompletion`, `Completed`, `EstimatedEffortHours`, `ActualEffortHours`, `SchedulePriorityGroup`, `PriorityInGroup`, `Personal`, `DependencyCount`, `Dependencies`, `LastUpdateTS`) VALUES
(1, 1, 1, 'Create a personal planner program that allows task mangement using QT and C++.', NULL, 0, 0, '2026-01-26 14:49:02', '2026-01-26', '2026-01-26', NULL, '2026-04-30', NULL, 1000, 500, 1, 1, 0, 0, NULL, '2026-01-26 14:49:02'),
(2, 1, 1, 'Add creation of notes in Database.', NULL, 0, 0, '2026-01-26 15:18:38', '2026-01-27', '2026-01-26', NULL, '2026-01-27', NULL, 4, 0, 1, 2, 0, 0, NULL, '2026-01-26 15:18:38'),
(3, 1, 1, 'Add editing of note to update database.', NULL, 0, 0, '2026-01-26 15:19:47', '2026-01-27', '2026-01-26', NULL, '2026-01-27', NULL, 4, 0, 1, 3, 0, 0, NULL, '2026-01-26 15:19:47'),
(4, 1, 1, 'Get toenails trimmed', NULL, 0, 0, '2026-01-26 15:21:21', '2026-01-28', '2026-01-28', NULL, '2026-01-28', NULL, 1, 0, 3, 1, 0, 0, NULL, '2026-01-26 15:21:21'),
(5, 1, 1, 'Connect goal creation and editing to database.', NULL, 0, 0, '2026-01-26 15:23:03', '2026-01-29', '2026-01-28', NULL, '2026-01-29', NULL, 4, 0, 1, 4, 0, 0, NULL, '2026-01-26 15:23:03'),
(6, 1, 1, 'Implement task editing.', NULL, 0, 0, '2026-01-26 15:24:39', '2026-01-30', '2026-01-29', NULL, '2026-01-30', NULL, 8, 0, 1, 5, 0, 0, NULL, '2026-01-26 15:24:39');

--
-- Dumping data for table `UserNotes`
--

INSERT INTO `UserNotes` (`idUserNotes`, `UserID`, `NotationDateTime`, `Content`, `LastUpdate`) VALUES
(1, 1, '2026-01-27 15:38:10', 'Testing adding a new note!', '2026-01-27 15:38:10');

--
-- Dumping data for table `UserProfile`
--

COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
