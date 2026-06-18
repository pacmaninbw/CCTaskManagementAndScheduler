--
-- Database: `testPTSDB`
--
# USE `testPTSDB`;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`AddTestUsers`;

CREATE PROCEDURE `testPTSDB`.`AddTestUsers`()

BEGIN

CALL AddUser(1, 'Black', 'Paul', 'A', 'paul.black@blacksw.com', 'pacmaninbw', 'mYfAv0r1t3Pas5MorD', '8:30 AM;5:00 PM;1;1;1;0;');
CALL AddUser(1, 'Black', 'Paul2', 'B', 'pcfriends@blacksw.com', 'BlackPaulA', 'pacmaninbw', '8:30 AM;5:00 PM;1;1;1;0;');
CALL AddUser(1, 'Shinny', 'Eric', 'Y', 'EricShinny@google.com', 'ShinnyEricY', 'ShinnyEricY', '8:30 AM;5:00 PM;1;1;1;0;');

END$$

DELIMITER ;


DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`AddTestTasks`;

CREATE PROCEDURE `testPTSDB`.`AddTestTasks`()

BEGIN

CALL AddTask(1, 1, 'Create a personal planner program that allows task mangement using QT and C++.', NULL, 3, '2026-02-20', '2026-02-20', '2026-02-23', '2026-04-30', NULL, 1000, 500, 1, 1, 0, 24, '2;3;5;10;12;13;14;15;19;20;24;25;26;27;30;31;33;35;37;38;39;40;43;46;');
CALL AddTask(1, 1, 'Add creation of notes in Database.', 1, 4, '2026-02-28', '2026-02-28', NULL, '2026-02-28', '2026-03-14', 8, 8, 1, 3, 0, 0, NULL);
CALL AddTask(1, 1, 'Add editing of note to update database.', 1, 4, '2026-02-28', '2026-02-28', NULL, '2026-02-28', '2026-03-14', 4, 4, 1, 4, 0, 0, NULL);
CALL AddTask(1, 1, 'Get toenails trimmed', NULL, 4, '2026-02-07', '2026-02-07', NULL, '2026-02-07', '2026-02-21', 1, 0, 3, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Connect goal creation and editing to database.', 1, 0, '2026-03-08', '2026-03-08', NULL, '2026-03-08', NULL, 6, 0, 1, 5, 0, 0, NULL);
CALL AddTask(1, 1, 'Implement task editing.', NULL, 4, '2026-02-07', '2026-02-07', NULL, '2026-02-07', '2026-02-21', 8, 0, 1, 5, 0, 0, NULL);
CALL AddTask(1, 1, 'Redisplay schedule after adding or editing a schedule item.', NULL, 4, '2026-02-16', '2026-02-14', '2026-02-22', '2026-02-22', '2026-02-22', 4, 6, 1, 4, 0, 0, NULL);
CALL AddTask(1, 1, 'Redisplay tasks after adding or editing a task.', NULL, 4, '2026-02-17', '2026-02-16', NULL, '2026-02-17', '2026-02-22', 4, 0, 1, 3, 0, 0, NULL);
CALL AddTask(1, 1, 'Sort tasks by group priority and priority.', NULL, 4, '2026-02-15', '2026-02-13', '2026-02-21', '2026-02-16', '2026-02-21', 6, 4, 1, 3, 0, 0, NULL);
CALL AddTask(1, 1, 'Implement editing of existing tasks', 1, 3, '2026-03-08', '2026-03-08', '2026-02-21', '2026-03-08', NULL, 8, 24, 1, 9, 0, 0, NULL);
CALL AddTask(1, 1, 'Implement editing of schedule.', NULL, 4,'2026-02-19', '2026-02-18', NULL, '2026-02-19', '2026-02-21', 8, 0, 1, 5, 0, 0, NULL);
CALL AddTask(1, 1, 'Finish the ability to edit tasks. Implement select parent task, add dependencies, display of dependencies.', 1, 3, '2026-03-11', '2026-03-08', '2026-03-07', '2026-03-11', NULL, 16, 20, 1, 8, 0, 1, '44;');
CALL AddTask(1, 1, 'Figure out how to use signals and slots to implement table view updates.', 0, 0, '2026-03-06', '2026-03-06', NULL, '2026-03-06', NULL, 4, 1, 2, 5, 0, 0, NULL);
CALL AddTask(1, 1, 'Clear selections after table edits.', 1, 4, '2026-03-06', '2026-03-06', NULL, '2026-03-06', '2026-03-20', 4, 1, 2, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Create Task View Table for select parent task', 1, 4, '2026-02-28', '2026-02-28', '2026-03-06', '2026-02-28', '2026-03-14', 8, 8, 1, 11, 0, 0, NULL);
CALL AddTask(1, 1, 'Create Requirement system to augment the planning system', NULL, 0, '2026-05-31', '2026-04-30', NULL, '2026-05-31', NULL, 300, 0, 3, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Implement user preferences in planner', NULL, 0, '2026-03-04', '2026-03-01', NULL, '2026-03-04', NULL, 24, 0, 3, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Find a Silver Sneakers program   ', NULL, 0, '2026-03-06', '2026-03-06', NULL, '2026-03-06', NULL, 4, 0, 2, 4, 0, 0, NULL);
CALL AddTask(1, 1, 'Add alternate methods of connecting to the database and logging in. Use environment variable and or command line flags.', 1, 3, '2026-03-08', '2026-03-08', '2026-03-06', '2026-03-08', NULL, 16, 20, 1, 6, 0, 0, NULL);
CALL AddTask(1, 1, 'Add the ability to repeat schedule items on a daily basis.', 1, 0, '2026-03-06', '2026-03-06', NULL, '2026-03-06', NULL, 16, 0, 2, 2, 0, 0, NULL);
CALL AddTask(1, 1, 'Add the ability to find an old schedule item and add it again to a new schedule', NULL, 0, '2026-02-27', '2026-02-27', NULL, '2026-02-27', NULL, 16, 0, 3, 2, 0, 0, NULL);
CALL AddTask(1, 1, 'Add the ability to reuse locations in the schedule.', NULL, 0, '2026-02-27', '2026-02-27', NULL, '2026-02-27', NULL, 24, 0, 3, 3, 0, 0, NULL);
CALL AddTask(1, 1, 'Freeze Chicken fillets', NULL, 4, '2026-03-05', '2026-02-27', NULL, '2026-02-27', '2026-03-03', 1, 0.25, 1, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Fix bug in scheduler where late appointments appear in the next day schedule.', 0, 4, '2026-02-28', '2026-02-28', NULL, '2026-02-28', '2026-03-07', 8, 1.5, 1, 4, 0, 0, NULL);
CALL AddTask(1, 1, 'Debug parent task id not being set in the database when the parent has been selected', 0, 4, '2026-03-01', '2026-03-01', '2026-03-01', '2026-03-01', '2026-03-06', 2, 3, 1, 4, 0, 0, NULL);
CALL AddTask(1, 1, 'Add production test database contents to unit/integration tests.2 ', 0, 0, '2026-03-01', '2026-03-01', NULL, '2026-03-01', NULL, 2, 0, 3, 4, 0, 0, NULL);
CALL AddTask(1, 1, 'Schedule date is based on GMT rather than local time, this causes some schedule items to appear on the wrong date.', 0, 4, '2026-03-03', '2026-03-03', NULL, '2026-03-03', '2026-03-07', 2, 1.5, 1, 2, 0, 0, NULL);
CALL AddTask(1, 1, 'Get a replacement blood pressure monitor', NULL, 4, '2026-03-04', '2026-03-04', NULL, '2026-03-04', '2026-03-06', 1, 1, 1, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Find a dentist. Make an appointment', NULL, 0, '2026-03-05', '2026-03-05', NULL, '2026-03-05', NULL, 3, 0, 3, 5, 0, 0, NULL);
CALL AddTask(1, 1, 'Add the ability to move tasks up and down in the todo list.', 1, 0, '2026-03-06', '2026-03-06', NULL, '2026-03-06', NULL, 16, 0, 2, 3, 0, 0, NULL);
CALL AddTask(1, 1, 'Prevent duplicate dependent tasks from occurring.', 1, 0, '2026-03-11', '2026-03-10', NULL, '2026-03-11', NULL, 4, 0, 1, 7, 0, 0, NULL);
CALL AddTask(1, 1, 'Do the laundry', NULL, 0, '2026-03-09', '2026-03-07', NULL, '2026-03-10', NULL, 3, 0, 3, 6, 0, 0, NULL);
CALL AddTask(1, 1, 'Change PG to Cat, short for category in Dashboard display task list', 1, 4, '2026-03-11', '2026-03-10', NULL, '2026-03-11', '2026-03-10', 1, 0.25, 3, 6, 0, 0, NULL);
CALL AddTask(1, 1, 'Buy proper tools for grill pan maintenance\n1) oil brush\n2) soft cloths for washing pan', NULL, 4, '2026-03-07', '2026-03-07', NULL, '2026-03-07', '2026-03-09', 2, 1, 2, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Rework the build and the code so that the GuiModels are not required.', 1, 4, '2026-03-21', '2026-03-12', '2026-03-14', '2026-03-21', '2026-03-19', 3, 23, 1, 3, 0, 1, '41;');
CALL AddTask(1, 1, 'Create a personal website using WordPress.', NULL, 0, '2026-03-27', '2026-03-23', NULL, '2026-03-28', NULL, 32, 0, 1, 2, 0, 0, NULL);
CALL AddTask(1, 1, 'Provide a method to sort the potential parent tasks in the select parent task dialog.', 1, 0, '2026-03-09', '2026-03-09', NULL, '2026-03-09', NULL, 6, 0, 2, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Change PG to Category in the dashboard task list.', 1, 4, '2026-03-09', '2026-03-09', NULL, '2026-03-09', '2026-03-09', 1, 0, 2, 7, 0, 0, NULL);
CALL AddTask(1, 1, 'Create an Organizational editor', 1, 0, '2026-03-09', '2026-03-09', NULL, '2026-03-09', NULL, 24, 0, 1, 4, 0, 0, NULL);
CALL AddTask(1, 1, 'Change the Date time editor field for a time edit field for schedule items start and end time', 1, 4, '2026-03-10', '2026-03-10', '2026-03-11', '2026-03-10', '2026-03-12', 2, 8, 1, 2, 0, 0, NULL);
CALL AddTask(1, 1, 'Change all pointers to model classes to database table IDs wherever possible', 35, 4, '2026-03-18', '2026-03-18', '2026-03-18', '2026-03-18', '2026-03-19', 4, 8, 1, 10, 0, 0, NULL);
CALL AddTask(1, 1, 'Testing the new push button interface for add task', NULL, 4, '2026-03-20', '2026-03-20', '2026-03-20', '2026-03-20', '2026-03-20', 1, 0.25, 1, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Remove dependencies on TaskModel from user dashboard', 1, 4, '2026-03-21', '2026-03-21', '2026-03-21', '2026-03-21', '2026-03-21', 1, 1, 1, 1, 0, 0, NULL);
CALL AddTask(1, 1, 'Show parent task name in task editor after it is selected.', 12, 4, '2026-03-21', '2026-03-21', NULL, '2026-03-21', '2026-03-21', 2, 0.5, 1, 9, 0, 1, '45;');
CALL AddTask(1, 1, 'Test add parent task display', 44, 4, '2026-03-21', '2026-03-21', NULL, '2026-03-21', '2026-03-21', 1, 0.25, 1, 10, 0, 0, NULL);
CALL AddTask(1, 1, 'Create a non-modal pop-up with similar contents for the user to select as the basis for a new schedule item', 1, 0, '2026-03-22', '2026-03-22', NULL, '2026-03-22', NULL, 24, 2, 1, 3, 0, 0, NULL);

END$$

DELIMITER ;

-- --------------------------------------------------------
DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`AddTestNotes`;

CREATE PROCEDURE `testPTSDB`.`AddTestNotes`()

BEGIN


INSERT INTO `testPTSDB`.`UserNotes` (`idUserNotes`, `UserID`, `NotationDateTime`, `Content`, `LastUpdate`) VALUES
(1, 1, '2026-03-12 13:51:37', 'This is a test of the add note dialog!', '2026-03-12 13:51:37'),
(2, 1, '2026-03-14 13:23:50', 'Add updateNoteTable when updating user or date', '2026-03-14 13:23:50'),
(3, 1, '2026-03-14 13:35:41', 'Connect note edits to handlers.', '2026-03-14 13:35:41'),
(4, 1, '2026-03-14 13:36:40', 'Change note table, last update is not necessary. Completed', '2026-03-14 13:36:40'),
(5, 1, '2026-03-14 13:37:20', 'Add symbolic constant for column count in all tables.', '2026-03-14 13:37:20'),
(6, 1, '2026-03-14 13:38:11', 'Sort notes by time added.', '2026-03-14 13:38:11'),
(7, 1, '2026-03-14 14:05:04', 'Include contents of note in note editor. ', '2026-03-14 14:05:04'),
(8, 1, '2026-03-14 14:05:45', 'Correct time in note to local time.', '2026-03-14 14:05:45'),
(9, 1, '2026-03-15 13:15:37', 'Qt doesn\'t allow shared pointers in createIndex()', '2026-03-15 13:15:37'),
(10, 1, '2026-03-15 13:16:35', 'Need to change editor dialogs to take raw pointers rather than shared pointers. It works.', '2026-03-15 13:16:35'),
(11, 1, '2026-03-16 13:01:18', 'User raw pointers in tables, by getting the raw pointers from the shared pointers!', '2026-03-16 13:01:18'),
(12, 1, '2026-03-16 16:19:59', 'Creator and assigned to name fields are blank in task editor. Parent task description is corrupted in task editor.', '2026-03-16 16:19:59'),
(13, 1, '2026-03-17 13:38:25', 'Passing pointers doesn\'t seem like such a good idea, at least in the task editor. Try passing database table id\'s and letting the editor retrieve the data.', '2026-03-17 13:38:25'),
(14, 1, '2026-03-18 13:20:39', 'Convert from class pointer to database table id wherever possible.', '2026-03-18 13:20:39'),
(15, 1, '2026-03-18 16:22:17', 'Need to debug core dump, expected location is in the creation of the note table.', '2026-03-18 16:22:17'),
(16, 1, '2026-03-19 11:42:16', 'Make sure to test add user before merging changes into master branch', '2026-03-19 11:42:16'),
(17, 1, '2026-03-19 11:49:02', 'Remove unnecessary GUI Model files, run build, see what still breaks.', '2026-03-19 11:49:02'),
(18, 1, '2026-03-19 13:13:53', 'Goal editor dialog doesn\'t do anything.', '2026-03-19 13:13:53'),
(19, 1, '2026-03-19 14:38:10', 'Removed all Gui Models. Merged branches!', '2026-03-19 14:38:10'),
(20, 1, '2026-03-20 12:28:58', 'Create a WBS for the website work.\n1) check to see what I want to keep from the previous work\n2) Download the latest WordPress version\n3) Install WordPress on Ubuntu 24.04', '2026-03-20 12:28:58'),
(21, 1, '2026-03-20 12:38:16', 'Move object creation/addition into the various group boxes, remove the menu at the top, or at least decrease the size of the menu. Testing.', '2026-03-20 12:38:16'),
(22, 1, '2026-03-20 15:25:45', 'Testing add a note!', '2026-03-20 15:25:45'),
(23, 1, '2026-03-20 15:40:06', 'Look into resizing push buttons based on text size for the dashboard buttons.', '2026-03-20 15:40:06'),
(24, 1, '2026-03-21 12:48:29', 'Check Verizon usage.', '2026-03-21 12:48:29'),
(25, 1, '2026-03-21 16:14:50', 'Need to add an index for Location in schedule item!', '2026-03-21 16:14:50'),
(26, 1, '2026-03-21 16:16:05', 'Need to figure out how to use location in repeat schedule items.', '2026-03-21 16:16:05');

END$$

DELIMITER ;


-- --------------------------------------------------------
DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`AddTestSchedule`;

CREATE PROCEDURE `testPTSDB`.`AddTestSchedule`()

BEGIN


INSERT INTO `UserScheduleItem` (`idUserScheduleItem`, `UserID`, `StartDateTime`, `EndDateTime`, `Title`, `Personal`, `Location`, `CreatedTS`, `LastUpdateTS`, `Hidden`) VALUES
(1, 1, '2026-02-05 15:29:05', '2026-02-05 15:29:05', 'CDC Chinese New Year luncheon', 1, 'China Buffet', '2026-02-05 15:29:05', '2026-02-05 15:29:05', 0),
(2, 1, '2026-02-05 15:31:02', '2026-02-05 15:31:02', 'Dr. Lakdawala', 1, '8641 Wilshire Blvd', '2026-02-05 15:31:02', '2026-02-05 15:31:02', 0),
(3, 1, '2026-02-05 15:32:35', '2026-02-05 15:32:35', 'Dr. Aftergood', 1, '99 N. La Cienaga', '2026-02-05 15:32:35', '2026-02-05 15:32:35', 0),
(4, 1, '2026-02-05 15:33:42', '2026-02-05 15:33:42', 'Review current status of Code Review', 0, 'Starbucks', '2026-02-05 15:33:42', '2026-02-05 15:33:42', 0),
(5, 1, '2026-02-07 12:30:00', '2026-02-07 12:30:00', 'Review Code Review Status', 0, 'Starbucks', '2026-02-07 13:35:40', '2026-02-07 13:35:40', 0),
(6, 1, '2026-02-07 13:00:00', '2026-02-07 14:30:00', 'Debug schedule viewer', 0, 'Starbucks', '2026-02-07 13:36:51', '2026-02-07 13:36:51', 0),
(7, 1, '2026-02-09 13:00:00', '2026-02-09 13:30:00', 'Code Review Moderator Overwatch', 0, 'Starbucks', '2026-02-09 14:58:02', '2026-02-09 14:58:02', 0),
(8, 1, '2026-02-09 13:30:00', '2026-02-09 16:30:00', 'Finish implementing planner day schedule', 0, 'Starbucks', '2026-02-09 14:59:41', '2026-02-09 14:59:41', 0),
(9, 1, '2026-02-09 18:00:00', '2026-02-09 18:30:00', 'Eat Breakfast', 1, 'Home', '2026-02-09 15:50:19', '2026-02-09 15:50:19', 0),
(10, 1, '2026-02-10 12:30:00', '2026-02-10 13:00:00', 'Code Review Moderator Oversite', 0, 'Starbucks', '2026-02-10 02:43:03', '2026-02-10 02:43:03', 0),
(11, 1, '2026-02-10 13:00:00', '2026-02-10 13:15:00', 'Check email and LinkedIn', 1, 'Starbucks', '2026-02-10 02:44:14', '2026-02-10 02:44:14', 0),
(12, 1, '2026-02-10 13:30:00', '2026-02-10 16:30:00', 'Implement sorting of scheduled item', 0, NULL, '2026-02-10 02:45:34', '2026-02-10 02:45:34', 0),
(13, 1, '2026-02-11 12:30:00', '2026-02-11 13:00:00', 'Code Review Moderator Oversite', 0, 'Starbucks', '2026-02-10 02:46:58', '2026-02-10 02:46:58', 0),
(14, 1, '2026-02-11 15:00:00', '2026-02-11 16:00:00', 'Shave and shower', 1, 'Home', '2026-02-10 02:48:18', '2026-02-10 02:48:18', 0),
(15, 1, '2026-02-11 18:00:00', '2026-02-11 19:00:00', 'Dr. Lakdawala', 1, '8641 WilShire Blvd.', '2026-02-10 02:49:57', '2026-02-10 02:49:57', 0),
(16, 1, '2026-02-10 17:15:00', '2026-02-10 17:45:00', 'Shower and shave', 1, 'Home', '2026-02-10 14:41:00', '2026-02-10 14:41:00', 0),
(17, 1, '2026-02-10 18:00:00', '2026-02-10 18:30:00', 'Eat breakfast', 1, 'Home', '2026-02-10 14:41:58', '2026-02-10 14:41:58', 0),
(18, 1, '2026-02-12 12:15:00', '2026-02-12 12:30:00', 'Starbuck Coffee', 0, NULL, '2026-02-12 19:47:14', '2026-02-12 19:47:14', 0),
(19, 1, '2026-02-12 12:30:00', '2026-02-12 13:00:00', 'Code Review Moderator Oversight', 0, NULL, '2026-02-12 19:48:06', '2026-02-12 19:48:06', 0),
(20, 1, '2026-02-12 13:00:00', '2026-02-12 16:30:00', 'Recode schedule generation', 0, 'Starbucks', '2026-02-12 19:49:17', '2026-02-12 19:49:17', 0),
(21, 1, '2026-02-12 18:00:00', '2026-02-12 19:00:00', 'Breakfast and Science Fiction', 0, 'Home', '2026-02-12 19:50:43', '2026-02-12 19:50:43', 0),
(22, 1, '2026-02-12 20:15:00', '2026-02-12 10:00:00', 'Debug scheduling issues', 0, 'home', '2026-02-12 19:51:34', '2026-02-12 19:51:34', 0),
(23, 1, '2026-02-13 19:30:00', '2026-02-13 21:30:00', 'CDC Chinese New Year Lunch', 1, 'China Buffet', '2026-02-13 14:29:34', '2026-02-13 14:29:34', 0),
(24, 1, '2026-02-13 12:15:00', '2026-02-13 12:30:00', 'Check Vitals ', 1, 'Home', '2026-02-13 14:30:47', '2026-02-13 14:30:47', 0),
(25, 1, '2026-02-13 12:45:00', '2026-02-13 13:15:00', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-02-13 14:32:23', '2026-02-13 14:32:23', 0),
(26, 1, '2026-02-13 13:30:00', '2026-02-13 15:00:00', 'Implement get local midnight', 0, 'Starbucks', '2026-02-13 14:33:44', '2026-02-13 14:33:44', 0),
(27, 1, '2026-02-13 16:30:00', '2026-02-13 17:30:00', 'Breakfast and Science Fiction', 1, 'Home', '2026-02-13 14:35:03', '2026-02-13 14:35:03', 0),
(28, 1, '2026-02-13 18:00:00', '2026-02-13 19:00:00', 'Shave and shower, clean cloths', 1, 'Home', '2026-02-13 14:36:02', '2026-02-13 14:36:02', 0),
(29, 1, '2026-02-14 12:30:00', '2026-02-14 13:00:00', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-02-14 14:19:38', '2026-02-14 14:19:38', 0),
(30, 1, '2026-02-14 13:00:00', '2026-02-14 15:00:00', 'Implement sort tasks by group priority and priority', 0, 'Starbucks', '2026-02-14 14:21:07', '2026-02-14 14:21:07', 0),
(31, 1, '2026-02-14 17:00:00', '2026-02-14 18:00:00', 'Breakfast and Science Fiction', 0, 'Home', '2026-02-14 14:22:58', '2026-02-14 14:22:58', 0),
(32, 1, '2026-02-16 13:30:00', '2026-02-16 14:00:00', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-02-17 01:24:44', '2026-02-17 01:24:44', 0),
(33, 1, '2026-02-16 14:00:00', '2026-02-16 17:30:00', 'Implement task editing.', 0, 'Starbucks', '2026-02-17 01:25:56', '2026-02-17 01:25:56', 0),
(34, 1, '2026-02-16 17:30:00', '2026-02-16 18:00:00', 'Get bananas and blueberrys', 1, 'Ralph\'s Markey', '2026-02-17 01:26:52', '2026-02-17 01:26:52', 0),
(35, 1, '2026-02-16 18:15:00', '2026-02-16 19:30:00', 'Breakfast and Science Fiction', 1, 'Home', '2026-02-17 01:27:48', '2026-02-17 01:27:48', 0),
(36, 1, '2026-02-17 11:45:00', '2026-02-17 12:00:00', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-17 01:40:13', '2026-02-17 01:40:13', 0),
(37, 1, '2026-02-17 12:15:00', '2026-02-17 12:45:00', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-02-17 01:41:30', '2026-02-17 01:41:30', 0),
(38, 1, '2026-02-17 13:00:00', '2026-02-17 16:30:00', 'Debug schedule item editing', 0, 'Starbucks', '2026-02-17 01:42:49', '2026-02-17 01:42:49', 0),
(39, 1, '2026-02-18 12:15:00', '2026-02-18 12:30:00', 'Check vital signs, log data on iPhone and in spreadsheet', 0, 'Home', '2026-02-18 13:57:01', '2026-02-18 13:57:01', 0),
(40, 1, '2026-02-18 12:30:00', '2026-02-18 12:45:00', 'Coffee at Starbucks!!!', 0, 'Starbucks', '2026-02-18 13:58:20', '2026-02-18 13:58:20', 0),
(41, 1, '2026-02-18 12:45:00', '2026-02-18 13:15:00', 'Code Review moderator oversight', 0, 'Starbucks', '2026-02-18 13:59:30', '2026-02-18 13:59:30', 0),
(42, 1, '2026-02-18 13:15:00', '2026-02-18 16:30:00', 'Debug schedule item editor segV.', 0, 'Starbucks', '2026-02-18 14:00:44', '2026-02-18 14:00:44', 0),
(43, 1, '2026-02-18 13:15:00', '2026-02-18 16:30:00', 'Debug schedule item editor segV.', 0, 'Starbucks', '2026-02-18 16:59:26', '2026-02-18 16:59:26', 0),
(44, 1, '2026-02-18 12:45:00', '2026-02-18 13:15:00', 'Code Review moderator oversight', 0, 'Starbucks', '2026-02-18 16:59:30', '2026-02-18 16:59:30', 0),
(45, 1, '2026-02-18 12:30:00', '2026-02-18 12:45:00', 'Coffee at Starbucks!!!', 1, 'Starbucks', '2026-02-18 16:59:39', '2026-02-18 16:59:39', 0),
(46, 1, '2026-02-18 12:15:00', '2026-02-18 12:30:00', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-18 16:59:56', '2026-02-18 16:59:56', 0),
(47, 1, '2026-02-18 18:00:00', '2026-02-18 19:00:00', 'Breakfast and Neal Asher', 1, NULL, '2026-02-19 01:44:17', '2026-02-19 01:44:17', 0),
(48, 1, '2026-02-19 12:15:00', '2026-02-19 12:30:00', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-19 15:55:36', '2026-02-19 15:55:36', 0),
(49, 1, '2026-02-19 12:30:00', '2026-02-19 12:45:00', 'Get coffee at starbucks, chat with people', 0, 'Starbucks', '2026-02-19 15:56:36', '2026-02-19 16:03:05', 0),
(50, 1, '2026-02-19 12:45:00', '2026-02-19 13:15:00', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-02-19 15:57:47', '2026-02-19 15:57:47', 0),
(51, 1, '2026-02-19 13:15:00', '2026-02-19 16:00:00', 'Add schedule item update integration test. Debug schedule item update', 0, 'Starbucks', '2026-02-19 15:59:19', '2026-02-19 16:05:15', 0),
(52, 1, '2026-02-19 19:00:00', '2026-02-19 20:00:00', 'Dr. David Aftergood, checkup', 1, '99. N. La Cienega', '2026-02-19 16:00:22', '2026-02-19 16:00:22', 0),
(53, 1, '2026-02-19 20:00:00', '2026-02-19 21:30:00', 'Bruch at Canters Deli', 1, 'Canters Deli', '2026-02-19 16:01:24', '2026-02-19 16:01:24', 0),
(54, 1, '2026-02-19 17:00:00', '2026-02-19 17:59:59', 'Shave and shower, prepare for doctor appointment.', 1, 'Home', '2026-02-19 16:27:12', '2026-02-19 16:27:12', 0),
(55, 1, '2026-02-20 12:00:00', '2026-02-20 12:15:00', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-20 02:00:14', '2026-02-20 02:07:13', 0),
(56, 1, '2026-02-20 12:30:00', '2026-02-20 12:45:00', 'Coffee Starbucks, talk to people ', 1, 'Starbucks', '2026-02-20 02:01:41', '2026-02-20 02:01:41', 0),
(57, 1, '2026-02-20 12:45:00', '2026-02-20 13:15:00', 'Code Review Moderator Oversite ', 0, 'Starbucks', '2026-02-20 02:02:57', '2026-02-20 02:02:57', 0),
(58, 1, '2026-02-20 13:15:00', '2026-02-20 16:30:00', 'Get all fields working on Task editing. Especially completing tasks and changing status ', 0, 'Starbucks', '2026-02-20 02:06:01', '2026-02-20 02:06:01', 0),
(59, 1, '2026-02-20 17:00:00', '2026-02-20 18:30:00', 'Breakfast and Science Fiction', 1, 'Home', '2026-02-20 02:06:27', '2026-02-20 02:09:58', 0),
(60, 1, '2026-02-20 21:30:00', '2026-02-20 21:59:59', 'Lunch and science fiction ', 1, 'home', '2026-02-20 02:08:47', '2026-02-20 02:08:47', 0),
(61, 1, '2026-02-20 19:00:00', '2026-02-20 19:59:59', 'Pay VISA credit card and health insurance', 1, 'home', '2026-02-20 02:09:32', '2026-02-20 02:09:32', 0),
(62, 1, '2026-02-21 12:30:00', '2026-02-21 12:45:00', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-21 14:19:48', '2026-02-21 14:19:48', 0),
(63, 1, '2026-02-21 12:45:00', '2026-02-21 13:00:00', 'Get coffee at Starbucks, say hello to people', 1, 'Starbucks', '2026-02-21 14:21:57', '2026-02-21 14:21:57', 0),
(64, 1, '2026-02-21 13:00:00', '2026-02-21 13:30:00', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-02-21 14:23:09', '2026-02-21 14:23:09', 0),
(65, 1, '2026-02-21 13:30:00', '2026-02-21 16:30:00', 'Implement full task editing.', 0, 'Starbucks', '2026-02-21 14:24:27', '2026-02-21 14:24:27', 0),
(66, 1, '2026-02-21 17:00:00', '2026-02-21 17:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-02-21 14:25:15', '2026-02-21 14:25:15', 0),
(67, 1, '2026-02-21 21:00:00', '2026-02-21 21:59:59', 'Implement task editing', 0, 'Home', '2026-02-21 21:54:33', '2026-02-21 21:54:33', 0),
(68, 1, '2026-02-22 12:30:00', '2026-02-22 12:45:00', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-22 16:01:18', '2026-02-22 16:03:11', 0),
(69, 1, '2026-02-22 13:00:00', '2026-02-22 13:15:00', 'Get coffee at Starbucks, say hello to everyone.', 1, 'Starbucks', '2026-02-22 16:03:05', '2026-02-22 16:03:05', 0),
(70, 1, '2026-02-22 13:30:00', '2026-02-22 14:00:00', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-02-22 16:04:34', '2026-02-22 16:04:34', 0),
(71, 1, '2026-02-22 14:00:00', '2026-02-22 16:30:00', 'Get redisplay of tasks and schedule after adding or editing working.', 0, 'Starbucks', '2026-02-22 16:08:34', '2026-02-22 16:08:34', 0),
(72, 1, '2026-02-22 17:00:00', '2026-02-22 17:30:00', 'Buy blue berries at Ralph\'s', 1, 'Ralph\'s market', '2026-02-22 16:09:53', '2026-02-22 16:30:58', 0),
(73, 1, '2026-02-22 18:00:00', '2026-02-22 19:30:00', 'Breakfast and Science Fiction ', 1, 'Home', '2026-02-22 16:10:35', '2026-02-22 16:19:40', 0),
(74, 1, '2026-02-23 12:00:00', '2026-02-23 12:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-22 16:23:39', '2026-02-22 16:25:54', 0),
(75, 1, '2026-02-23 12:15:00', '2026-02-23 12:29:59', 'Get coffee at Starbucks, say hello to everyone.', 1, 'Starbucks', '2026-02-22 16:25:48', '2026-02-22 16:25:48', 0),
(76, 1, '2026-02-23 12:30:00', '2026-02-23 13:00:00', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-02-22 16:28:07', '2026-02-22 16:28:07', 0),
(77, 1, '2026-02-23 13:00:00', '2026-02-23 16:45:00', 'Personal Planner software development ', 0, 'Starbucks', '2026-02-22 16:29:40', '2026-02-22 16:29:40', 0),
(78, 1, '2026-02-23 18:00:00', '2026-02-23 19:30:00', 'Breakfast and Science Fiction', 1, 'Home', '2026-02-22 16:30:27', '2026-02-23 14:41:18', 0),
(79, 1, '2026-02-24 12:00:00', '2026-02-24 12:15:00', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-23 14:30:35', '2026-02-23 14:30:35', 0),
(80, 1, '2026-02-24 12:15:00', '2026-02-24 12:30:00', 'Coffee at Starbcucks, say hello to people. ', 1, 'Starbucks', '2026-02-23 14:32:08', '2026-02-23 14:32:08', 0),
(81, 1, '2026-02-24 12:30:00', '2026-02-24 13:00:00', 'Code Review Moderator Oversight ', 0, 'Starbucks', '2026-02-23 14:34:16', '2026-02-23 14:34:16', 0),
(82, 1, '2026-02-24 13:00:00', '2026-02-24 15:29:59', 'Personal Planner Software Development ', 0, 'Starbucks', '2026-02-23 14:35:27', '2026-02-23 14:35:27', 0),
(83, 1, '2026-02-24 15:30:00', '2026-02-24 17:15:00', 'Breakfast, shave, shower, prepare for appointment with Dr. Wachtel ', 1, 'Home', '2026-02-23 14:37:09', '2026-02-23 14:37:09', 0),
(84, 1, '2026-02-24 18:45:00', '2026-02-24 19:15:00', 'Sleep medicine Dr Appointment, Dr. Wachtel ', 1, '8635 W. Third Street.', '2026-02-23 14:38:47', '2026-02-23 14:38:47', 0),
(85, 1, '2026-02-23 16:30:00', '2026-02-23 17:59:59', 'Take care to American Tire, get tires checked', 1, 'American Tire\n22910 Hawthorne Blvd.', '2026-02-23 14:43:45', '2026-02-23 14:44:03', 0),
(86, 1, '2026-02-23 20:00:00', '2026-02-23 20:59:59', 'Prepare chicken for freezer', 1, 'Home', '2026-02-23 14:44:45', '2026-02-23 14:44:45', 0),
(87, 1, '2026-02-25 11:45:00', '2026-02-25 12:00:00', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-25 12:31:43', '2026-02-25 12:31:43', 0),
(88, 1, '2026-02-25 12:00:00', '2026-02-25 12:15:00', 'Coffee at Starbucks, say hello to people', 1, 'Starbucks', '2026-02-25 12:33:20', '2026-02-25 12:33:20', 0),
(89, 1, '2026-02-25 12:15:00', '2026-02-25 12:29:59', 'Plan day using PP3Rework', 1, 'Starbucks', '2026-02-25 12:34:44', '2026-02-25 12:34:44', 0),
(90, 1, '2026-02-25 12:30:00', '2026-02-25 12:59:59', 'Code Review Moderator Oversight ', 0, 'Starbucks', '2026-02-25 12:37:02', '2026-02-25 12:37:02', 0),
(91, 1, '2026-02-25 13:00:00', '2026-02-25 16:59:59', 'Implement the select parent task dialog', 0, 'Starbucks', '2026-02-25 12:38:12', '2026-02-25 12:38:12', 0),
(92, 1, '2026-02-25 17:00:00', '2026-02-25 18:29:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-02-25 12:39:17', '2026-02-25 12:39:17', 0),
(93, 1, '2026-02-25 19:00:00', '2026-02-25 19:59:59', 'Food shopping, get 1 week of groceries', 1, 'Redondo Beach Ralph\'s Market', '2026-02-25 12:41:23', '2026-02-25 12:41:23', 0),
(94, 1, '2026-02-25 22:00:00', '2026-02-25 22:59:59', 'Lunch Science Fiction', 1, 'Home', '2026-02-25 12:42:01', '2026-02-25 12:42:01', 0),
(95, 1, '2026-02-26 12:15:00', '2026-02-26 12:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-26 13:17:03', '2026-02-26 13:17:03', 0),
(96, 1, '2026-02-26 12:30:00', '2026-02-26 12:44:59', 'Get coffee at Starbuck, say hello to everyone', 1, 'Starbucks', '2026-02-26 13:18:18', '2026-02-26 13:18:18', 0),
(97, 1, '2026-02-26 12:45:00', '2026-02-26 13:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-02-26 13:19:27', '2026-02-26 13:19:27', 0),
(98, 1, '2026-02-26 13:15:00', '2026-02-26 13:29:59', 'Plan day using prototype personal planner ', 1, 'Starbucks', '2026-02-26 13:21:04', '2026-02-26 13:21:04', 0),
(99, 1, '2026-02-26 13:30:00', '2026-02-26 16:29:59', 'Implement generic task table as a base class for multiple views of task tables. ', 0, 'Starbucks', '2026-02-26 13:24:06', '2026-02-26 13:24:06', 0),
(100, 1, '2026-02-26 17:00:00', '2026-02-26 17:59:59', 'Grocery shopping at Redondo Beach Ralph\'s Market', 1, 'Redondo Beach Ralph\'s Market', '2026-02-26 13:26:11', '2026-02-26 13:26:11', 0),
(101, 1, '2026-02-26 18:00:00', '2026-02-26 18:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-02-26 13:26:57', '2026-02-26 13:26:57', 0),
(102, 1, '2026-02-26 22:00:00', '2026-02-26 22:59:59', 'Lunch and Science Fiction', 1, 'Home', '2026-02-26 13:27:52', '2026-02-26 13:27:52', 0),
(103, 1, '2026-02-26 23:00:00', '2026-02-26 23:59:59', 'Food preparation', 1, 'Home', '2026-02-26 13:28:29', '2026-02-26 13:28:29', 0),
(104, 1, '2026-02-27 12:15:00', '2026-02-27 12:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-27 12:50:36', '2026-02-27 12:50:36', 0),
(105, 1, '2026-02-27 12:30:00', '2026-02-27 12:44:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-02-27 12:51:29', '2026-02-27 12:51:29', 0),
(106, 1, '2026-02-27 12:45:00', '2026-02-27 12:59:59', 'Plan day using personal planner/organizer ', 1, 'Starbucks', '2026-02-27 12:53:06', '2026-02-27 12:53:06', 0),
(107, 1, '2026-02-27 13:00:00', '2026-02-27 13:29:59', 'Code Review Moderator Oversight ', 0, 'Starbucks', '2026-02-27 12:54:18', '2026-02-27 12:54:18', 0),
(108, 1, '2026-02-27 13:30:00', '2026-02-27 16:59:59', 'Finish implementing the Select Parent Task dialog', 0, 'Starbucks', '2026-02-27 12:56:03', '2026-02-27 12:56:03', 0),
(109, 1, '2026-02-27 17:00:00', '2026-02-27 17:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-02-27 12:56:31', '2026-02-27 12:56:31', 0),
(110, 1, '2026-02-27 18:00:00', '2026-02-27 18:59:59', 'Call Bill McCandless', 1, 'Home', '2026-02-27 12:57:06', '2026-02-27 12:57:06', 0),
(111, 1, '2026-02-27 19:00:00', '2026-02-27 20:29:59', 'Food preparation, cut chicken into 1/3 pound fillets. Chop vegetables ', 1, 'Home', '2026-02-27 12:59:01', '2026-02-27 12:59:01', 0),
(112, 1, '2026-02-27 22:00:00', '2026-02-27 22:59:59', 'Lunch and science fiction', 1, 'Home', '2026-02-27 12:59:44', '2026-02-27 12:59:44', 0),
(113, 1, '2026-02-28 12:00:00', '2026-02-28 12:14:29', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-02-27 23:18:10', '2026-02-27 23:18:10', 0),
(114, 1, '2026-02-28 12:15:00', '2026-02-28 12:29:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-02-27 23:19:05', '2026-02-27 23:19:05', 0),
(115, 1, '2026-02-28 12:30:00', '2026-02-28 12:44:59', 'Plan day using personal planner/organizer ', 1, 'Starbucks', '2026-02-27 23:20:02', '2026-02-27 23:20:02', 0),
(116, 1, '2026-02-28 13:00:00', '2026-02-28 13:29:59', 'Code Review Moderator Oversight ', 0, 'Starbucks', '2026-02-27 23:21:14', '2026-02-27 23:21:14', 0),
(117, 1, '2026-02-28 13:30:00', '2026-02-28 16:29:59', 'Find where something is being freed twice during select parent.', 0, 'Starbucks', '2026-02-27 23:22:40', '2026-02-27 23:22:40', 0),
(118, 1, '2026-02-28 17:00:00', '2026-02-28 17:59:59', 'Breakfast and science fiction', 1, 'Home', '2026-02-27 23:23:13', '2026-02-27 23:23:13', 0),
(119, 1, '2026-03-05 18:00:00', '2026-03-05 18:14:59', 'Freeze Chicken Fillets', 1, 'Home', '2026-02-27 23:25:53', '2026-03-05 13:59:48', 0),
(120, 1, '2026-03-04 22:00:00', '2026-03-04 22:59:59', 'Freeze Chicken Fillets', 1, 'Home', '2026-02-27 23:26:21', '2026-02-27 23:26:21', 0),
(121, 1, '2026-02-28 21:00:00', '2026-02-28 21:59:59', 'Lunch and Science Fiction', 1, 'Home', '2026-02-28 12:52:03', '2026-02-28 12:52:03', 0),
(122, 1, '2026-03-01 12:00:00', '2026-03-01 12:15:00', 'Check Vital Signs log data on iPhone and in spreadsheet ', 1, 'Home', '2026-03-01 12:42:53', '2026-03-01 12:42:53', 0),
(123, 1, '2026-03-01 11:45:00', '2026-03-01 11:59:59', 'Prepare some chicken breasts for cooking.', 1, 'Home', '2026-03-01 12:43:54', '2026-03-01 12:43:54', 0),
(124, 1, '2026-03-01 12:30:00', '2026-03-01 12:44:59', 'Coffee at Starbucks, say hello to everyone', 1, 'Starbucks', '2026-03-01 12:45:34', '2026-03-01 12:45:34', 0),
(125, 1, '2026-03-01 12:45:00', '2026-03-01 12:59:59', 'Plan day using electronic personal planner ', 1, 'Starbucks', '2026-03-01 12:46:39', '2026-03-01 12:46:39', 0),
(126, 1, '2026-03-01 13:00:00', '2026-03-01 13:29:59', 'Code Review Moderator Oversight ', 0, 'Starbucks', '2026-03-01 12:48:39', '2026-03-01 12:48:39', 0),
(127, 1, '2026-03-01 13:30:00', '2026-03-01 16:59:59', 'Fix bugs in planner: 1) Save the parent task id to the child task. 2) copy constructor for TaskModel is really needed?', 0, 'Starbucks', '2026-03-01 12:51:20', '2026-03-01 12:51:20', 0),
(128, 1, '2026-03-01 17:00:00', '2026-03-01 17:59:59', 'Breakfast and science fiction.', 1, 'Home', '2026-03-01 12:52:35', '2026-03-01 12:52:35', 0),
(129, 1, '2026-03-01 21:00:00', '2026-03-01 21:59:59', 'Lunch and Science Fiction', 1, 'Home', '2026-03-01 16:14:41', '2026-03-01 16:14:41', 0),
(130, 1, '2026-03-03 11:15:00', '2026-03-03 11:29:59', 'Shave shave shave', 1, 'Home', '2026-03-03 12:24:08', '2026-03-03 12:24:08', 0),
(131, 1, '2026-03-03 11:30:00', '2026-03-03 11:44:59', 'Freeze chicken fillets', 1, 'Home', '2026-03-03 12:24:52', '2026-03-03 12:24:52', 0),
(132, 1, '2026-03-03 11:45:00', '2026-03-03 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-03 12:26:04', '2026-03-03 12:26:04', 0),
(133, 1, '2026-03-03 12:00:00', '2026-03-03 12:14:59', 'Coffee, say hellp to everyone ', 1, 'Starbucks', '2026-03-03 12:27:01', '2026-03-03 12:27:01', 0),
(134, 1, '2026-03-03 12:15:00', '2026-03-03 12:29:59', 'Plan day, check email  ', 1, 'Starbucks', '2026-03-03 12:28:04', '2026-03-03 12:28:04', 0),
(135, 1, '2026-03-03 12:30:00', '2026-03-03 12:59:59', 'Code Review Moderator Oversight ', 0, 'Starbucks', '2026-03-03 12:29:13', '2026-03-03 12:29:13', 0),
(136, 1, '2026-03-03 13:00:00', '2026-03-03 17:29:59', 'Finish implementing getting login credentials from environment variables ', 0, 'Starbucks', '2026-03-03 12:31:10', '2026-03-03 12:31:10', 0),
(137, 1, '2026-03-03 17:30:00', '2026-03-03 18:59:59', 'Breakfast and science fiction', 1, 'Home', '2026-03-03 12:32:05', '2026-03-03 12:32:05', 0),
(138, 1, '2026-03-03 19:00:00', '2026-03-03 19:59:59', 'Order grill pan and new frying pan with covers online', 1, 'Home', '2026-03-03 12:32:50', '2026-03-03 12:32:50', 0),
(139, 1, '2026-03-03 22:00:00', '2026-03-03 22:59:59', 'Lunch and Science Fiction	 ', 1, 'Home', '2026-03-03 12:33:29', '2026-03-03 12:33:29', 0),
(140, 1, '2026-03-03 20:00:00', '2026-03-03 21:59:59', 'Fix limit on Dashboard Task View', 0, 'Home', '2026-03-03 12:34:28', '2026-03-03 12:34:43', 0),
(141, 1, '2026-03-04 17:00:00', '2026-03-04 17:59:59', 'Pedicure and massage', 1, 'Lovely Nails', '2026-03-04 02:58:22', '2026-03-04 02:58:22', 0),
(142, 1, '2026-03-04 12:00:00', '2026-03-04 12:14:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-03-04 02:59:29', '2026-03-04 02:59:29', 0),
(143, 1, '2026-03-04 12:15:00', '2026-03-04 12:44:59', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-03-04 03:00:26', '2026-03-04 03:00:26', 0),
(144, 1, '2026-03-04 12:45:00', '2026-03-04 14:59:59', 'User Login from environment variables ', 0, 'Starbucks', '2026-03-04 03:01:34', '2026-03-04 03:01:34', 0),
(145, 1, '2026-03-04 16:00:00', '2026-03-04 16:59:59', 'shave and shower, prep for pedicure', 1, 'Home', '2026-03-04 03:02:07', '2026-03-04 03:02:07', 0),
(146, 1, '2026-03-04 11:45:00', '2026-03-04 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-04 03:03:57', '2026-03-04 03:03:57', 0),
(147, 1, '2026-03-05 01:00:00', '2026-03-05 01:59:59', 'Start rice cooker', 1, 'Home', '2026-03-04 03:04:31', '2026-03-04 03:04:31', 0),
(148, 1, '2026-03-05 00:00:00', '2026-03-05 00:59:59', 'Check schedule date', 0, NULL, '2026-03-04 03:05:01', '2026-03-04 03:05:01', 0),
(149, 1, '2026-03-05 12:45:00', '2026-03-05 12:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-05 13:21:51', '2026-03-05 13:21:51', 0),
(150, 1, '2026-03-05 13:00:00', '2026-03-05 13:29:59', 'Code Review Moderator Oversight ', 0, 'Starbucks', '2026-03-05 13:56:04', '2026-03-05 13:56:04', 0),
(151, 1, '2026-03-05 13:30:00', '2026-03-05 13:59:59', 'Log issues for planner on GitHub ', 0, 'Starbucks', '2026-03-05 13:57:05', '2026-03-05 13:57:05', 0),
(152, 1, '2026-03-05 14:00:00', '2026-03-05 16:59:59', 'Make logging into the planner user friendly	 ', 0, 'Starbucks', '2026-03-05 13:58:23', '2026-03-05 13:58:23', 0),
(153, 1, '2026-03-05 17:00:00', '2026-03-05 17:59:59', 'Breakfast and science fiction.', 1, 'Home', '2026-03-05 13:59:12', '2026-03-05 13:59:12', 0),
(154, 1, '2026-03-06 18:30:00', '2026-03-06 19:59:59', 'Recover lost property from the state of California', 1, 'Home', '2026-03-05 14:01:11', '2026-03-06 12:36:22', 0),
(155, 1, '2026-03-05 22:00:00', '2026-03-05 22:59:59', 'Lunch and  Science Fiction', 1, 'Home', '2026-03-05 14:01:40', '2026-03-05 14:01:40', 0),
(156, 1, '2026-03-05 20:00:00', '2026-03-05 20:59:59', 'Replace blood pressure monitor.', 1, 'Costco', '2026-03-05 14:02:21', '2026-03-05 14:02:21', 0),
(157, 1, '2026-03-06 12:00:00', '2026-03-06 12:14:59', 'Coffee at Starbucks say hello to everyone ', 1, 'Starbucks', '2026-03-06 12:25:48', '2026-03-06 12:25:48', 0),
(158, 1, '2026-03-06 12:15:00', '2026-03-06 12:29:59', 'Plan the day using the electronic planner organizer', 1, 'Starbucks', '2026-03-06 12:27:02', '2026-03-06 12:27:02', 0),
(159, 1, '2026-03-06 11:15:00', '2026-03-06 11:44:59', 'Put together the new blood pressure monitor', 1, 'Home', '2026-03-06 12:28:04', '2026-03-06 12:28:04', 0),
(160, 1, '2026-03-06 11:45:00', '2026-03-06 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet ', 1, 'Home', '2026-03-06 12:29:01', '2026-03-06 12:29:01', 0),
(161, 1, '2026-03-06 12:30:00', '2026-03-06 12:59:59', 'Code Review moderator Oversight ', 0, 'Starbucks', '2026-03-06 12:30:43', '2026-03-06 12:30:43', 0),
(162, 1, '2026-03-06 13:00:00', '2026-03-06 16:59:59', 'Debug task views', 0, 'Starbucks', '2026-03-06 12:31:32', '2026-03-06 12:31:32', 0),
(163, 1, '2026-03-06 17:00:00', '2026-03-06 17:29:59', 'Get milk and  bananas ', 1, 'Ralph\'s Market', '2026-03-06 12:32:35', '2026-03-06 12:32:35', 0),
(164, 1, '2026-03-06 17:30:00', '2026-03-06 18:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-03-06 12:33:16', '2026-03-06 12:33:16', 0),
(165, 1, '2026-03-06 21:30:00', '2026-03-06 22:29:59', 'Lunch and science fiction ', 1, 'Home', '2026-03-06 12:35:39', '2026-03-06 12:35:39', 0),
(166, 1, '2026-03-09 17:00:00', '2026-03-09 17:59:59', 'Dr Arnold Cinman', 1, 'Tower Urology\n8635 W. 3rd Street', '2026-03-06 15:18:11', '2026-03-06 15:18:11', 0),
(167, 1, '2026-03-09 11:45:00', '2026-03-09 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-06 15:19:17', '2026-03-09 12:22:26', 0),
(168, 1, '2026-03-09 12:00:00', '2026-03-09 12:14:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-03-06 15:20:11', '2026-03-09 12:25:31', 0),
(169, 1, '2026-03-09 12:30:00', '2026-03-09 13:00:00', 'Finsh planning day', 1, 'Starbucks', '2026-03-06 15:21:05', '2026-03-09 12:24:33', 0),
(170, 1, '2026-03-09 11:30:00', '2026-03-09 11:59:59', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-03-06 15:21:51', '2026-03-06 15:21:51', 0),
(171, 1, '2026-03-10 10:45:00', '2026-03-10 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 0, NULL, '2026-03-06 15:23:12', '2026-03-10 12:24:53', 0),
(172, 1, '2026-03-10 16:30:00', '2026-03-10 17:19:59', 'Dr. AbdelKumar ', 1, 'Little Company of Mary', '2026-03-06 15:24:37', '2026-03-10 12:05:08', 0),
(173, 1, '2026-03-06 23:00:00', '2026-03-07 00:59:59', 'Debug schedule items showing up a day late. ', 0, 'Home', '2026-03-06 16:39:55', '2026-03-06 16:39:55', 0),
(174, 1, '2026-03-07 12:45:00', '2026-03-07 12:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, NULL, '2026-03-07 13:22:18', '2026-03-07 13:22:18', 0),
(175, 1, '2026-03-07 13:00:00', '2026-03-07 13:14:59', 'Coffee at Starbucks, say hello to everyone ', 1, NULL, '2026-03-07 13:23:47', '2026-03-07 13:23:47', 0),
(176, 1, '2026-03-07 13:15:59', '2026-03-07 13:29:59', 'Plan day using electronic organizer app ', 1, NULL, '2026-03-07 13:24:54', '2026-03-07 13:24:54', 0),
(177, 1, '2026-03-07 13:30:00', '2026-03-07 13:59:59', 'Code review moderator oversight ', 1, NULL, '2026-03-07 13:25:51', '2026-03-07 13:25:51', 0),
(178, 1, '2026-03-07 14:00:00', '2026-03-07 16:59:59', 'Fix schedule display issues', 0, 'Starbucks', '2026-03-07 13:26:54', '2026-03-07 13:26:54', 0),
(179, 1, '2026-03-07 17:00:00', '2026-03-07 17:59:59', 'Breakfast and science fiction', 0, 'Home', '2026-03-07 13:27:33', '2026-03-07 13:27:33', 0),
(180, 1, '2026-03-07 18:00:00', '2026-03-07 18:59:59', 'Wash the dishes', 1, 'home', '2026-03-07 13:27:53', '2026-03-07 13:27:53', 0),
(181, 1, '2026-03-08 03:00:00', '2026-03-08 03:59:59', 'Dinner at Norm\'s', 1, 'South Torrance Norm\'s Restaurant', '2026-03-07 13:28:55', '2026-03-07 13:28:55', 0),
(182, 1, '2026-03-08 12:00:00', '2026-03-08 12:14:59', 'Check Vital Signs log data on iPhone and in spreadsheet ', 1, 'Home', '2026-03-08 12:45:47', '2026-03-08 12:45:47', 0),
(183, 1, '2026-03-08 12:30:00', '2026-03-08 12:44:59', 'Coffee at Starbucks, say hello to everyone', 1, 'Starbucks', '2026-03-08 12:47:54', '2026-03-08 12:47:54', 0),
(184, 1, '2026-03-08 12:45:00', '2026-03-08 12:59:59', 'Plan day in electronic Planner', 1, 'Starbucks', '2026-03-08 12:48:47', '2026-03-08 12:48:47', 0),
(185, 1, '2026-03-08 13:00:00', '2026-03-08 13:29:59', 'Code Review Moderator Oversight ', 0, 'Starbucks', '2026-03-08 12:49:35', '2026-03-08 12:49:35', 0),
(186, 1, '2026-03-08 13:30:00', '2026-03-08 15:59:59', 'Electronic planner/organizer software development', 0, 'Starbucks', '2026-03-08 12:51:09', '2026-03-08 12:51:36', 0),
(187, 1, '2026-03-08 16:00:00', '2026-03-08 17:29:59', 'Breakfast and science fiction ', 1, 'Home', '2026-03-08 12:52:32', '2026-03-08 12:52:32', 0),
(188, 1, '2026-03-08 18:00:00', '2026-03-08 18:59:59', 'Prepare chicken for cooking', 1, 'Home', '2026-03-08 12:53:57', '2026-03-08 12:53:57', 0),
(189, 1, '2026-03-08 19:00:00', '2026-03-08 20:59:59', 'Grocery shopping ', 1, 'Redondo Beach Ralph\'s', '2026-03-08 12:54:50', '2026-03-08 12:54:50', 0),
(190, 1, '2026-03-08 21:00:00', '2026-03-08 21:59:59', 'Slice up veggies', 1, 'Home', '2026-03-08 12:56:11', '2026-03-08 12:56:11', 0),
(191, 1, '2026-03-09 14:30:00', '2026-03-09 15:29:59', 'Shave and shower', 1, 'Home', '2026-03-08 13:06:51', '2026-03-09 01:30:40', 0),
(192, 1, '2026-03-09 22:00:00', '2026-03-09 22:59:59', 'Cut chicken into fillets and marinate the chicken.', 1, 'Home', '2026-03-09 12:58:52', '2026-03-09 12:58:52', 0),
(193, 1, '2026-03-09 21:00:00', '2026-03-09 21:59:59', 'Wash the Dishes from yesterdays dinner.', 1, NULL, '2026-03-09 13:15:45', '2026-03-09 13:15:45', 0),
(194, 1, '2026-03-10 11:15:00', '2026-03-10 11:29:59', 'Coffee, say hello to everyone including the plumbers', 1, 'Starbucks', '2026-03-10 12:18:00', '2026-03-10 12:24:42', 0),
(195, 1, '2026-03-10 11:30:00', '2026-03-10 11:59:59', 'Code Review Moderator Oversight ', 0, 'Starbucks', '2026-03-10 12:18:50', '2026-03-10 12:24:32', 0),
(196, 1, '2026-03-10 12:00:00', '2026-03-10 12:29:59', 'Reprioritize task list ', 1, 'Starbucks', '2026-03-10 12:20:24', '2026-03-10 12:20:24', 0),
(197, 1, '2026-03-10 15:00:00', '2026-03-10 16:14:59', 'Take meds, shower and shave', 1, 'Home', '2026-03-10 12:21:08', '2026-03-10 12:24:23', 0),
(198, 1, '2026-03-10 17:30:00', '2026-03-10 18:59:59', 'Breakfast, science fiction ', 1, 'Home', '2026-03-10 12:21:54', '2026-03-10 12:21:54', 0),
(199, 1, '2026-03-10 19:00:00', '2026-03-10 19:59:59', 'Cut chicken into fillets, marinate the fillets.', 1, 'Home', '2026-03-10 12:23:14', '2026-03-10 12:23:14', 0),
(200, 1, '2026-03-10 18:15:00', '2026-03-10 18:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-11 12:19:12', '2026-03-11 12:59:37', 0),
(201, 1, '2026-03-11 11:33:00', '2026-03-11 11:45:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-03-11 12:21:04', '2026-03-11 15:01:42', 0),
(202, 1, '2026-03-11 11:45:00', '2026-03-11 12:15:59', 'Code Review moderator oversight', 0, 'Starbucks', '2026-03-11 12:21:55', '2026-03-11 12:21:55', 0),
(203, 1, '2026-03-11 12:15:00', '2026-03-11 12:30:59', 'Plan day using electronic planner', 1, 'Starbucks', '2026-03-11 12:22:52', '2026-03-11 14:58:02', 0),
(204, 1, '2026-03-11 12:30:00', '2026-03-11 15:59:59', 'Fix schedule editor issues', 0, 'Starbucks', '2026-03-11 12:24:20', '2026-03-11 12:24:20', 0),
(205, 1, '2026-03-11 16:00:00', '2026-03-11 17:30:59', 'Breakfast, science fiction ', 1, 'Home', '2026-03-11 12:26:14', '2026-03-11 12:26:14', 0),
(206, 1, '2026-03-11 18:00:00', '2026-03-11 18:15:59', 'Marinate chicken for tonights dinner', 1, 'Home', '2026-03-11 12:27:04', '2026-03-11 12:27:04', 0),
(207, 1, '2026-03-11 20:00:00', '2026-03-11 21:00:59', 'Lunch science fiction ', 1, 'Home', '2026-03-11 12:28:37', '2026-03-11 12:28:37', 0),
(208, 1, '2026-03-12 02:00:59', '2026-03-12 03:00:59', 'Dinner, wash dishes', 1, 'Home', '2026-03-11 12:32:36', '2026-03-11 14:57:21', 0),
(209, 1, '2026-03-11 19:00:00', '2026-03-11 19:59:59', 'Fix schedule editor issues', 0, 'Home', '2026-03-11 12:35:27', '2026-03-11 12:35:27', 0),
(210, 1, '2026-03-11 22:00:00', '2026-03-11 22:14:59', 'This is only a test, what happens', 0, NULL, '2026-03-11 15:02:46', '2026-03-11 15:02:46', 0),
(211, 1, '2026-03-12 11:45:00', '2026-03-12 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-12 12:47:26', '2026-03-12 12:47:26', 0),
(212, 1, '2026-03-12 12:15:00', '2026-03-12 12:29:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-03-12 12:48:40', '2026-03-12 12:48:40', 0),
(213, 1, '2026-03-12 12:30:00', '2026-03-12 12:59:59', 'Check email, plan day using electronic planner ', 1, 'Starbucks', '2026-03-12 12:49:56', '2026-03-12 12:49:56', 0),
(214, 1, '2026-03-12 13:00:00', '2026-03-12 13:14:59', 'Code Review moderator oversight', 0, 'Starbucks', '2026-03-12 12:50:49', '2026-03-12 12:50:49', 0),
(215, 1, '2026-03-12 13:15:00', '2026-03-12 13:59:59', 'Estimate effort to change schedule table display to use QDateTime.tolocaldatetime rather than current implementation ', 0, 'Starbucks', '2026-03-12 12:54:32', '2026-03-12 12:54:32', 0),
(216, 1, '2026-03-12 14:00:00', '2026-03-12 15:59:59', 'Planner development tasks as necessary.', 0, 'Starbucks', '2026-03-12 12:55:17', '2026-03-12 12:55:17', 0),
(217, 1, '2026-03-12 16:00:00', '2026-03-12 16:59:59', 'Breakfast, science fiction	', 1, 'Home', '2026-03-12 12:55:52', '2026-03-12 12:55:52', 0),
(218, 1, '2026-03-12 17:00:00', '2026-03-12 17:59:59', 'Marinate all the chicken in preparation for freezing', 1, 'Home', '2026-03-12 12:56:47', '2026-03-12 12:56:47', 0),
(219, 1, '2026-03-12 20:00:00', '2026-03-12 20:59:59', 'Lunch and science fiction', 1, 'Home', '2026-03-12 12:57:29', '2026-03-12 12:57:29', 0),
(220, 1, '2026-03-13 11:00:48', '2026-03-13 11:14:48', 'Check vital signs, log data on iPhone and in spreasheet', 1, 'Home', '2026-03-12 13:29:12', '2026-03-13 11:52:44', 0),
(221, 1, '2026-03-13 11:30:00', '2026-03-13 11:45:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-03-12 13:30:04', '2026-03-13 11:53:27', 0),
(222, 1, '2026-03-13 02:00:00', '2026-03-13 02:59:59', 'Dinner, wash dishes', 1, 'Home', '2026-03-12 13:31:26', '2026-03-12 13:31:26', 0),
(223, 1, '2026-03-13 01:45:00', '2026-03-13 01:59:59', 'Cook dinner ', 1, 'Home', '2026-03-12 13:31:50', '2026-03-12 13:31:50', 0),
(224, 1, '2026-03-13 11:15:00', '2026-03-13 11:29:59', 'Wash pots and pans from last nights dinner ', 1, 'Home', '2026-03-13 11:54:12', '2026-03-13 11:54:12', 0),
(225, 1, '2026-03-13 11:45:00', '2026-03-13 12:14:59', 'Read email, Plan day using PP3Rework app ', 1, 'Starbucks', '2026-03-13 11:55:14', '2026-03-13 11:55:14', 0),
(226, 1, '2026-03-13 12:15:00', '2026-03-13 12:44:59', 'Code Review Moderator oversight', 0, 'Starbucks', '2026-03-13 11:56:18', '2026-03-13 11:56:18', 0),
(227, 1, '2026-03-13 12:45:00', '2026-03-13 15:59:59', 'Add unit and integration test for new NoteList search ', 0, 'Starbucks', '2026-03-13 11:58:04', '2026-03-13 11:58:04', 0),
(228, 1, '2026-03-13 16:00:00', '2026-03-13 16:59:59', 'Grocery shopping', 0, NULL, '2026-03-13 11:58:26', '2026-03-13 11:58:26', 0),
(229, 1, '2026-03-13 17:00:00', '2026-03-13 17:59:59', 'Breakfast and Science Fiction ', 1, 'Home', '2026-03-13 11:59:02', '2026-03-13 11:59:02', 0),
(230, 1, '2026-03-13 18:00:00', '2026-03-13 18:59:59', 'Pay Amex credit card bill', 1, 'Home', '2026-03-13 11:59:40', '2026-03-13 11:59:40', 0),
(231, 1, '2026-03-13 21:00:00', '2026-03-13 21:59:59', 'Lunch, whatever ', 1, 'Home', '2026-03-13 12:03:06', '2026-03-13 12:03:06', 0),
(232, 1, '2026-03-14 10:45:00', '2026-03-14 10:59:59', 'Wash dishes from dinner ', 1, 'Home', '2026-03-14 12:04:47', '2026-03-14 12:04:47', 0),
(233, 1, '2026-03-14 11:00:00', '2026-03-14 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-14 12:06:15', '2026-03-14 12:06:15', 0),
(234, 1, '2026-03-14 11:30:00', '2026-03-14 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-03-14 12:07:12', '2026-03-14 12:07:12', 0),
(235, 1, '2026-03-14 11:45:00', '2026-03-14 11:59:59', 'Code Review moderator oversight', 0, 'Starbucks', '2026-03-14 12:08:12', '2026-03-14 12:08:12', 0),
(236, 1, '2026-03-14 12:00:00', '2026-03-14 12:14:59', 'Plan day, read email ', 1, 'Starbucks', '2026-03-14 12:08:54', '2026-03-14 12:08:54', 0),
(237, 1, '2026-03-14 12:15:00', '2026-03-14 15:59:59', 'Debug note table issues', 0, 'Starbucks', '2026-03-14 12:09:30', '2026-03-14 12:09:30', 0),
(238, 1, '2026-03-15 02:00:00', '2026-03-15 02:59:59', 'Dinner, split pea soup! ', 1, 'Norm\'s', '2026-03-14 12:10:15', '2026-03-14 12:10:15', 0),
(239, 1, '2026-03-14 16:00:00', '2026-03-14 16:59:59', 'Breakfast, science fiction', 1, 'Home', '2026-03-14 12:10:44', '2026-03-14 12:10:44', 0),
(240, 1, '2026-03-14 17:00:00', '2026-03-14 17:59:59', 'Freeze the chicken', 1, 'Home', '2026-03-14 12:11:07', '2026-03-14 12:11:07', 0),
(241, 1, '2026-03-15 11:00:00', '2026-03-15 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-15 11:45:03', '2026-03-15 11:45:03', 0),
(242, 1, '2026-03-15 11:30:00', '2026-03-15 11:59:59', 'Plan day, check email ', 1, 'Starbucks', '2026-03-15 11:46:25', '2026-03-15 11:46:25', 0),
(243, 1, '2026-03-15 12:00:00', '2026-03-15 12:29:59', 'Code review moderator oversight', 0, 'Starbucks', '2026-03-15 11:47:26', '2026-03-15 11:47:46', 0),
(244, 1, '2026-03-15 12:30:00', '2026-03-15 15:59:59', 'Continue removing Gui Models from planning software ', 0, 'Starbucks', '2026-03-15 11:49:34', '2026-03-15 11:49:34', 0),
(245, 1, '2026-03-15 16:00:00', '2026-03-15 16:14:59', 'Get blueberries and Onion powder ', 1, 'Vons', '2026-03-15 11:51:44', '2026-03-15 11:51:44', 0),
(246, 1, '2026-03-15 16:15:00', '2026-03-15 17:59:59', 'Breakfast and science fiction ', 1, 'Home', '2026-03-15 11:52:27', '2026-03-15 11:52:27', 0),
(247, 1, '2026-03-15 18:00:00', '2026-03-15 18:59:59', 'Freeze chicken!!!', 1, 'Home', '2026-03-15 11:53:45', '2026-03-15 11:53:45', 0),
(248, 1, '2026-03-15 19:00:00', '2026-03-15 20:59:59', 'Continue removing GuiModels ', 0, 'Home', '2026-03-15 11:54:26', '2026-03-15 11:54:26', 0),
(249, 1, '2026-03-16 11:30:00', '2026-03-16 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-16 12:05:14', '2026-03-16 12:05:14', 0),
(250, 1, '2026-03-16 11:15:00', '2026-03-16 11:29:59', 'Start diswash ', 1, 'Home', '2026-03-16 12:05:49', '2026-03-16 12:05:49', 0),
(251, 1, '2026-03-16 11:45:00', '2026-03-16 12:14:59', 'check email, plan day ', 1, 'Starbucks', '2026-03-16 12:06:55', '2026-03-16 12:06:55', 0),
(252, 1, '2026-03-16 12:15:00', '2026-03-16 12:30:59', 'Code review moderator oversight ', 0, 'Starbucks', '2026-03-16 12:08:27', '2026-03-16 12:08:27', 0),
(253, 1, '2026-03-16 12:30:00', '2026-03-16 15:59:59', 'Remove GuiTask from planner organizer project - Rework the build', 1, 'Starbucks', '2026-03-16 12:09:57', '2026-03-16 12:11:28', 0),
(254, 1, '2026-03-16 16:00:00', '2026-03-16 16:59:59', 'Breakfast and science fiction', 1, 'Home', '2026-03-16 12:10:27', '2026-03-16 12:10:27', 0),
(255, 1, '2026-03-16 18:00:00', '2026-03-16 18:59:59', 'Fix bugs introduced in morning session of remove GuiTaskModel', 0, 'Home', '2026-03-16 16:20:52', '2026-03-16 16:20:52', 0),
(256, 1, '2026-03-16 17:00:00', '2026-03-16 17:59:59', 'Marinate chicken for tonights dinner.', 1, 'Home', '2026-03-16 16:21:27', '2026-03-16 16:21:27', 0),
(257, 1, '2026-03-17 11:00:00', '2026-03-17 11:14:59', 'Wash yesterday\'s dishes ', 1, 'Home', '2026-03-17 12:10:17', '2026-03-17 12:10:17', 0),
(258, 1, '2026-03-17 11:15:00', '2026-03-17 11:29:59', 'check vital signs, log data on iPhone and in spreadsheet ', 1, 'Home', '2026-03-17 12:11:18', '2026-03-17 12:11:18', 0),
(259, 1, '2026-03-17 11:30:00', '2026-03-17 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-03-17 12:12:02', '2026-03-17 12:12:02', 0),
(260, 1, '2026-03-17 11:45:00', '2026-03-17 11:59:59', 'Code Review moderator oversight ', 0, 'Starbucks', '2026-03-17 12:13:20', '2026-03-17 12:13:20', 0),
(261, 1, '2026-03-17 12:00:00', '2026-03-17 12:29:59', 'Read email, plan day ', 1, 'Starbucks', '2026-03-17 12:15:05', '2026-03-17 12:15:05', 0),
(262, 1, '2026-03-17 12:15:00', '2026-03-17 15:59:59', 'Debug the task editor, the parent name string gets corrupted. ', 0, 'Starbucks', '2026-03-17 12:16:20', '2026-03-17 12:16:20', 0),
(263, 1, '2026-03-17 16:00:00', '2026-03-17 16:59:59', 'Breakfast and science fiction', 1, 'Home', '2026-03-17 12:26:07', '2026-03-17 12:26:07', 0),
(264, 1, '2026-03-18 11:30:00', '2026-03-18 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-18 13:22:10', '2026-03-18 13:22:10', 0),
(265, 1, '2026-03-18 11:45:00', '2026-03-18 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-03-18 13:22:56', '2026-03-18 13:22:56', 0),
(266, 1, '2026-03-18 12:00:00', '2026-03-18 12:14:59', 'Code Review moderator oversight ', 0, 'Starbucks', '2026-03-18 13:23:57', '2026-03-18 13:23:57', 0),
(267, 1, '2026-03-18 12:15:00', '2026-03-18 15:59:59', 'Change class pointers to database table ID\'s wherever possible', 0, 'Starbucks', '2026-03-18 13:25:02', '2026-03-18 13:25:02', 0),
(268, 1, '2026-03-18 16:00:00', '2026-03-18 16:14:59', 'Buy bananas and blue berries', 1, 'Ralph\'s Market', '2026-03-18 13:25:51', '2026-03-18 13:25:51', 0),
(269, 1, '2026-03-18 18:00:00', '2026-03-18 18:59:59', 'Debug core dump for note table.', 0, 'Home', '2026-03-18 16:21:32', '2026-03-18 16:21:32', 0),
(270, 1, '2026-03-19 11:00:00', '2026-03-19 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-19 11:39:21', '2026-03-19 11:39:21', 0),
(271, 1, '2026-03-19 11:15:00', '2026-03-19 11:29:59', 'Coffee, say hello to everyone', 1, 'Starbucks', '2026-03-19 11:40:02', '2026-03-19 11:40:02', 0),
(272, 1, '2026-03-19 11:30:00', '2026-03-19 11:59:59', 'Read email, plan day ', 1, 'Starbucks', '2026-03-19 11:40:41', '2026-03-19 11:40:41', 0),
(273, 1, '2026-03-19 12:00:00', '2026-03-19 12:29:59', 'Code Review moderator oversight ', 0, 'Starbucks', '2026-03-19 11:43:03', '2026-03-19 11:43:03', 0),
(274, 1, '2026-03-19 12:15:00', '2026-03-19 15:59:59', 'Finish removing all GuiModels ', 0, 'Starbucks', '2026-03-19 11:45:12', '2026-03-19 11:45:12', 0),
(275, 1, '2026-03-19 16:30:00', '2026-03-19 17:59:59', 'Breakfast and science fiction ', 1, 'Home', '2026-03-19 11:46:05', '2026-03-19 11:46:05', 0),
(276, 1, '2026-03-19 18:00:00', '2026-03-19 18:59:59', 'Prepare to pay Visa card, check checking balance move funds as necessary', 1, 'Home', '2026-03-19 11:47:04', '2026-03-19 11:47:04', 0),
(277, 1, '2026-03-20 11:00:00', '2026-03-20 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-20 11:46:26', '2026-03-20 12:19:13', 0),
(278, 1, '2026-03-20 11:30:00', '2026-03-20 11:44:59', 'Coffee, Say hello to everyone ', 1, 'Starbucks', '2026-03-20 11:47:50', '2026-03-20 11:47:50', 0),
(279, 1, '2026-03-20 11:45:00', '2026-03-20 11:59:59', 'Read email, plan day', 1, 'Starbucks', '2026-03-20 11:48:50', '2026-03-20 11:48:50', 0),
(280, 1, '2026-03-20 12:00:00', '2026-03-20 12:29:59', 'Code Review moderator oversight ', 0, 'Starbucks', '2026-03-20 11:50:11', '2026-03-20 11:50:11', 0),
(281, 1, '2026-03-20 12:30:00', '2026-03-20 15:59:59', 'PP3Rework development Tasks, TBD', 0, 'Starbucks', '2026-03-20 11:51:24', '2026-03-20 11:51:24', 0),
(282, 1, '2026-03-20 16:00:00', '2026-03-20 16:59:59', 'Breakfast and science fiction', 1, 'Home', '2026-03-20 11:52:00', '2026-03-20 11:52:00', 0),
(283, 1, '2026-03-21 01:00:00', '2026-03-21 01:59:59', 'Start rice cooker', 1, 'Home', '2026-03-20 11:54:13', '2026-03-20 11:54:13', 0),
(284, 1, '2026-03-20 17:00:00', '2026-03-20 17:59:59', 'Fix some bugs.', 0, 'Home', '2026-03-20 13:11:06', '2026-03-20 13:11:06', 0),
(285, 1, '2026-03-20 15:15:53', '2026-03-20 15:29:53', 'Testing add an event', 0, 'Starbucks', '2026-03-20 15:26:51', '2026-03-20 15:26:51', 0),
(286, 1, '2026-03-21 12:00:00', '2026-03-21 12:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-21 12:43:04', '2026-03-21 12:43:04', 0),
(287, 1, '2026-03-21 12:15:00', '2026-03-21 12:29:59', 'Coffee, chat with people ', 1, 'Starbucks', '2026-03-21 12:43:54', '2026-03-21 12:43:54', 0),
(288, 1, '2026-03-21 12:30:00', '2026-03-21 12:59:59', 'Check email, clear notices on LinkedIn, plan day ', 1, 'Starbucks', '2026-03-21 12:45:00', '2026-03-21 12:45:00', 0),
(289, 1, '2026-03-21 13:00:00', '2026-03-21 13:14:59', 'Code review moderator oversight', 0, 'Starbucks', '2026-03-21 12:45:42', '2026-03-21 12:45:42', 0),
(290, 1, '2026-03-21 13:15:00', '2026-03-21 15:59:59', 'PP3Rework Development Tasks ', 0, 'Starbucks', '2026-03-21 12:49:26', '2026-03-21 12:49:26', 0),
(291, 1, '2026-03-22 01:30:00', '2026-03-22 02:59:59', 'Dinner, Pea soup!', 1, 'Norm\'s South Torrance', '2026-03-21 12:50:31', '2026-03-21 12:50:31', 0),
(292, 1, '2026-03-21 16:00:00', '2026-03-21 16:59:59', 'Breakfast, science fiction', 1, 'Home', '2026-03-21 12:51:11', '2026-03-21 12:51:11', 0),
(293, 1, '2026-03-22 11:45:00', '2026-03-22 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-22 12:28:18', '2026-03-22 12:28:18', 0),
(294, 1, '2026-03-22 12:00:00', '2026-03-22 12:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-03-22 12:29:03', '2026-03-22 12:29:03', 0),
(295, 1, '2026-03-22 12:15:00', '2026-03-22 12:29:59', 'Read email, plan day', 1, 'Starbucks', '2026-03-22 12:29:41', '2026-03-22 12:29:41', 0),
(296, 1, '2026-03-22 12:30:00', '2026-03-22 12:44:59', 'Code review moderator oversight ', 0, 'Starbucks', '2026-03-22 12:31:22', '2026-03-22 12:31:22', 0),
(297, 1, '2026-03-22 12:45:00', '2026-03-22 15:59:59', 'Allow a previously scheduled event to be used as source for a new event', 0, 'Starbucks', '2026-03-22 12:32:25', '2026-03-22 12:32:25', 0),
(298, 1, '2026-03-22 16:00:00', '2026-03-22 16:59:59', 'Breakfast, science fiction ', 1, 'Home', '2026-03-22 12:33:12', '2026-03-22 12:33:12', 0),
(299, 1, '2026-03-21 22:00:00', '2026-03-21 22:59:59', 'Lunch at Steve\'s burgers', 1, 'Steve\'s Burgers', '2026-03-22 13:39:40', '2026-03-22 13:39:40', 0),
(300, 1, '2026-03-23 11:15:00', '2026-03-23 11:29:59', 'Load dishwash, start dishwash ', 1, 'Home', '2026-03-23 12:14:55', '2026-03-23 12:14:55', 0),
(301, 1, '2026-03-23 11:30:00', '2026-03-23 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-23 12:16:08', '2026-03-23 12:17:34', 0),
(302, 1, '2026-03-23 11:45:00', '2026-03-23 11:59:59', 'Coffee, say hello to everyone', 1, 'Starbucks', '2026-03-23 12:17:21', '2026-03-23 12:17:21', 0),
(303, 1, '2026-03-23 12:00:00', '2026-03-23 12:29:59', 'Read email, plan day', 1, 'Starbucks', '2026-03-23 12:18:38', '2026-03-23 12:18:38', 0),
(304, 1, '2026-03-23 12:30:00', '2026-03-23 12:45:59', 'Code Review moderator oversight ', 0, 'Starbucks', '2026-03-23 12:19:28', '2026-03-23 12:20:42', 0),
(305, 1, '2026-03-23 13:00:00', '2026-03-23 15:59:59', 'Fix automated testing of the database after adding actual data to the integration tests. ', 0, 'Starbucks', '2026-03-23 12:20:35', '2026-03-23 12:20:35', 0),
(306, 1, '2026-03-23 16:00:00', '2026-03-23 16:59:59', 'Breakfast, science fiction', 1, 'Home', '2026-03-23 12:21:02', '2026-03-23 12:21:02', 0),
(307, 1, '2026-03-23 17:00:00', '2026-03-23 17:59:59', 'Marinate chicken', 1, 'Home', '2026-03-23 12:23:10', '2026-03-23 12:23:10', 0),
(308, 1, '2026-03-24 10:45:00', '2026-03-24 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-24 10:41:31', '2026-03-24 10:41:31', 0),
(309, 1, '2026-03-24 11:00:00', '2026-03-24 11:14:59', 'Coffee, say hello to everyone', 1, 'Starbucks', '2026-03-24 10:42:32', '2026-03-24 10:42:32', 0),
(310, 1, '2026-03-24 11:15:00', '2026-03-24 11:29:59', 'Read email, plan day', 1, 'Starbucks', '2026-03-24 10:43:42', '2026-03-24 10:44:09', 0),
(311, 1, '2026-03-24 11:30:00', '2026-03-24 11:44:59', 'Code Review moderator oversight ', 0, 'Starbucks', '2026-03-24 10:44:45', '2026-03-24 10:44:45', 0),
(312, 1, '2026-03-24 12:00:00', '2026-03-24 15:59:59', 'Fix unit and integration tests', 0, 'Starbucks', '2026-03-24 10:45:24', '2026-03-24 10:45:34', 0),
(313, 1, '2026-03-24 16:00:00', '2026-03-24 16:59:59', 'Get blue berries and vegtbles', 1, 'Ralph\'s Market', '2026-03-24 10:46:27', '2026-03-24 10:47:02', 0),
(314, 1, '2026-03-24 17:00:00', '2026-03-24 17:59:59', 'Breakfast and science fiction', 1, 'Home', '2026-03-24 10:46:59', '2026-03-24 10:46:59', 0),
(315, 1, '2026-03-24 18:00:00', '2026-03-24 18:59:59', 'Marinate Chicken', 1, 'Home', '2026-03-24 11:36:42', '2026-03-24 11:36:42', 0),
(316, 1, '2026-03-25 01:45:00', '2026-03-25 01:59:59', 'Cook dinner ', 1, 'Home', '2026-03-24 11:37:41', '2026-03-24 11:37:41', 0),
(317, 1, '2026-03-24 19:00:00', '2026-03-24 19:59:59', 'Check to see why the Cedars bill wasn\'t paid.', 1, 'Home', '2026-03-24 11:38:31', '2026-03-24 11:39:02', 0),
(318, 1, '2026-03-26 11:30:00', '2026-03-26 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-26 12:44:23', '2026-03-26 12:44:23', 0),
(319, 1, '2026-03-26 11:45:00', '2026-03-26 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-03-26 12:45:18', '2026-03-26 12:45:18', 0);
INSERT INTO `UserScheduleItem` (`idUserScheduleItem`, `UserID`, `StartDateTime`, `EndDateTime`, `Title`, `Personal`, `Location`, `CreatedTS`, `LastUpdateTS`, `Hidden`) VALUES
(320, 1, '2026-03-26 12:00:00', '2026-03-26 12:29:59', 'Code review moderator oversight ', 0, 'Starbucks', '2026-03-26 12:46:25', '2026-03-26 12:46:25', 0),
(321, 1, '2026-03-26 12:30:00', '2026-03-26 12:59:59', 'Check email, plan day ', 1, 'Starbucks', '2026-03-26 12:47:12', '2026-03-26 12:47:12', 0),
(322, 1, '2026-03-26 13:00:00', '2026-03-26 15:59:59', 'Work on stored procedure necessary to implement the non-modal pop-up with similar contents', 0, 'Starbucks', '2026-03-26 12:48:29', '2026-03-26 12:48:29', 0),
(323, 1, '2026-03-26 16:00:00', '2026-03-26 16:14:59', 'Get bananas ', 1, 'Ralph\'s Market', '2026-03-26 12:49:18', '2026-03-26 12:49:18', 0),
(324, 1, '2026-03-26 16:30:00', '2026-03-26 17:59:59', 'Breakfast, science fiction', 1, 'Home', '2026-03-26 12:49:47', '2026-03-26 12:49:47', 0),
(325, 1, '2026-03-26 18:00:00', '2026-03-26 19:59:59', 'Passover grocery shopping', 1, 'Gelson\'s market', '2026-03-26 12:50:37', '2026-03-26 12:50:37', 0),
(326, 1, '2026-03-26 20:45:00', '2026-03-26 21:14:59', 'Lunch, science fiction', 1, 'Home', '2026-03-26 12:52:03', '2026-03-26 12:52:03', 0),
(327, 1, '2026-03-26 21:15:00', '2026-03-26 22:59:59', 'Work on non-modal pop-up with similar content', 0, 'Home', '2026-03-26 12:52:59', '2026-03-26 12:52:59', 0),
(328, 1, '2026-03-26 20:00:00', '2026-03-26 20:59:59', 'Defrost chicken for Friday\'s dinner', 1, 'Home', '2026-03-26 12:54:02', '2026-03-26 12:54:02', 0),
(329, 1, '2026-03-27 01:30:36', '2026-03-27 02:59:36', 'Dinner, science fiction', 1, 'Panda Express', '2026-03-26 12:56:26', '2026-03-26 12:56:26', 0),
(330, 1, '2026-03-27 11:00:00', '2026-03-27 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home  ', '2026-03-27 11:42:45', '2026-03-27 11:42:45', 0),
(331, 1, '2026-03-27 11:15:00', '2026-03-27 11:29:59', 'Marinate chicken ', 1, 'Home', '2026-03-27 11:43:19', '2026-03-27 11:43:19', 0),
(332, 1, '2026-03-27 11:30:00', '2026-03-27 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-03-27 11:44:37', '2026-03-27 11:44:37', 0),
(333, 1, '2026-03-27 11:45:00', '2026-03-27 11:59:59', 'Read email, plan day ', 1, 'Starbucks', '2026-03-27 11:45:28', '2026-03-27 11:45:28', 0),
(334, 1, '2026-03-27 12:00:00', '2026-03-27 12:14:59', 'Code review moderator oversight ', 0, 'Starbucks', '2026-03-27 11:46:58', '2026-03-27 11:46:58', 0),
(335, 1, '2026-03-27 12:15:00', '2026-03-27 15:59:59', 'Debug non-modal pop-up with similar content testing ', 0, 'Starbucks', '2026-03-27 11:48:05', '2026-03-27 11:48:05', 0),
(336, 1, '2026-03-27 16:00:00', '2026-03-27 16:14:59', 'Marinate chicken ', 1, 'Home', '2026-03-27 11:49:43', '2026-03-27 11:49:43', 0),
(337, 1, '2026-03-27 16:15:00', '2026-03-27 17:29:59', 'Breakfast, science fiction', 1, 'Home', '2026-03-27 11:50:56', '2026-03-27 11:50:56', 0),
(338, 1, '2026-03-28 01:00:00', '2026-03-28 01:59:59', 'Cook rice for dinner preparation ', 1, 'Home', '2026-03-27 11:54:22', '2026-03-27 11:54:22', 0),
(339, 1, '2026-03-28 02:00:00', '2026-03-28 02:59:59', 'Cook dinner', 1, 'Home', '2026-03-27 11:54:40', '2026-03-27 11:54:40', 0),
(340, 1, '2026-03-27 17:30:00', '2026-03-27 17:59:59', 'Pick up meds at Costco, get gas ', 1, 'Costco', '2026-03-27 11:55:56', '2026-03-27 11:55:56', 0),
(341, 1, '2026-03-27 18:00:00', '2026-03-27 19:59:59', 'Non-modal pop up', 0, 'Home', '2026-03-27 11:56:53', '2026-03-27 11:56:53', 0),
(342, 1, '2026-03-27 20:00:00', '2026-03-27 20:59:59', 'A test of QTextEdit versus QPlainTextEdit', 0, 'Anywhere', '2026-03-27 15:39:39', '2026-03-27 15:39:39', 0),
(343, 1, '2026-03-28 11:15:00', '2026-03-28 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-28 11:52:53', '2026-03-28 11:52:53', 0),
(344, 1, '2026-03-28 11:30:00', '2026-03-28 11:44:59', 'Coffee, say hello to everyone', 1, 'Starbucks', '2026-03-28 11:54:41', '2026-03-28 11:54:41', 0),
(345, 1, '2026-03-28 11:45:00', '2026-03-28 12:14:59', 'Read email, plan day ', 1, 'Starbucks', '2026-03-28 11:56:09', '2026-03-28 11:56:16', 0),
(346, 1, '2026-03-28 12:15:00', '2026-03-28 12:29:59', 'Code review moderator oversight ', 0, 'Starbucks', '2026-03-28 11:57:02', '2026-03-28 11:57:02', 0),
(347, 1, '2026-03-28 12:30:00', '2026-03-28 15:59:59', 'Implement GUI portion of non-modal pop-up with similar contents for user', 0, 'Starbucks', '2026-03-28 11:58:22', '2026-03-28 11:58:22', 0),
(348, 1, '2026-03-28 16:00:00', '2026-03-28 16:59:59', 'Breakfast, science fiction ', 1, 'Starbucks', '2026-03-28 11:59:20', '2026-03-28 11:59:20', 0),
(349, 1, '2026-03-28 20:00:00', '2026-03-28 20:59:59', 'Lunch, science fiction ', 1, 'Home', '2026-03-28 12:34:18', '2026-03-28 12:34:18', 0),
(350, 1, '2026-03-29 11:00:00', '2026-03-29 11:14:59', 'Shut off all electronics to prevent power surge and protect the UPS. ', 1, 'Home', '2026-03-29 12:13:34', '2026-03-29 12:13:34', 0),
(351, 1, '2026-03-29 11:15:00', '2026-03-29 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-29 12:14:23', '2026-03-29 12:14:23', 0),
(352, 1, '2026-03-29 11:45:00', '2026-03-29 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-03-29 12:15:11', '2026-03-29 12:15:11', 0),
(353, 1, '2026-03-29 12:00:00', '2026-03-29 12:29:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-03-29 12:16:20', '2026-03-29 12:16:20', 0),
(354, 1, '2026-03-29 12:30:00', '2026-03-29 12:44:59', 'Code review moderator oversight ', 0, 'Starbucks', '2026-03-29 12:18:19', '2026-03-29 12:18:19', 0),
(355, 1, '2026-03-29 12:30:00', '2026-03-29 15:59:59', 'Implement non-modal pop-up with similar contents ', 0, NULL, '2026-03-29 12:19:25', '2026-03-29 12:19:25', 0),
(356, 1, '2026-03-29 16:00:00', '2026-03-29 16:59:59', 'Breakfast, science fiction', 1, 'IHOP', '2026-03-29 12:20:41', '2026-03-29 12:20:41', 0),
(357, 1, '2026-03-29 20:00:00', '2026-03-29 20:59:59', 'Lunch and  Science Fiction', 0, NULL, '2026-03-29 14:34:30', '2026-03-29 14:34:30', 0),
(358, 1, '2026-03-30 11:00:00', '2026-03-30 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-29 15:10:34', '2026-03-29 15:11:31', 0),
(359, 1, '2026-03-30 11:15:00', '2026-03-30 11:29:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-03-29 15:13:05', '2026-03-29 15:13:05', 0),
(360, 1, '2026-03-30 11:30:00', '2026-03-30 11:44:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-03-29 15:14:08', '2026-03-30 11:45:40', 0),
(361, 1, '2026-03-30 11:45:00', '2026-03-30 11:59:59', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-03-29 15:15:28', '2026-03-30 11:46:26', 0),
(362, 1, '2026-03-30 12:00:00', '2026-03-30 15:59:59', 'Experiment with QCompleter to see if it will do a better job than the current n0on-modal implementation', 0, 'Starbucks', '2026-03-29 15:17:17', '2026-03-30 11:46:55', 0),
(363, 1, '2026-03-31 12:00:00', '2026-03-31 12:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-03-29 15:35:51', '2026-03-31 13:17:39', 0),
(364, 1, '2026-03-31 12:15:00', '2026-03-31 12:29:59', 'Coffee at Starbucks, say hello to everyone', 1, 'Starbucks', '2026-03-31 13:18:41', '2026-03-31 13:18:41', 0),
(365, 1, '2026-03-31 12:30:00', '2026-03-31 12:59:59', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-03-31 13:19:25', '2026-03-31 13:19:25', 0),
(366, 1, '2026-03-31 13:00:00', '2026-03-31 13:30:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-03-31 13:20:09', '2026-03-31 13:20:09', 0),
(367, 1, '2026-03-31 13:30:00', '2026-03-31 15:59:59', 'Experiment with QCompleter to see if it will do a better job than the current n0on-modal implementation', 0, 'Starbucks', '2026-03-31 13:21:28', '2026-03-31 13:21:28', 0),
(368, 1, '2026-03-31 16:00:00', '2026-03-31 16:59:59', 'Breakfast, science fiction ', 1, 'Home', '2026-03-31 13:21:54', '2026-03-31 13:21:54', 0),
(369, 1, '2026-03-31 17:00:00', '2026-03-31 17:59:59', 'Grocery shopping at Redondo Beach Ralph\'s Market', 1, 'Ralph\'s Market', '2026-03-31 13:22:58', '2026-03-31 13:22:58', 0),
(370, 1, '2026-03-31 21:00:00', '2026-03-31 21:59:59', 'Lunch and science fiction ', 1, 'Home', '2026-03-31 13:25:28', '2026-03-31 13:25:28', 0),
(371, 1, '2026-04-01 01:30:00', '2026-04-01 02:29:59', 'Cook dinner ', 0, NULL, '2026-03-31 13:26:18', '2026-03-31 13:26:18', 0),
(372, 1, '2026-04-01 10:45:00', '2026-04-01 10:59:59', 'Check Vital Signs log data on iPhone and in spreadsheet ', 1, 'Home', '2026-04-01 11:32:45', '2026-04-01 11:32:45', 0),
(373, 1, '2026-04-01 11:00:00', '2026-04-01 11:14:59', 'Get coffee at Starbucks, say hello to everyone.', 1, 'Starbucks', '2026-04-01 11:33:56', '2026-04-01 11:33:56', 0),
(374, 1, '2026-04-01 11:15:00', '2026-04-01 11:45:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-01 11:34:58', '2026-04-01 11:34:58', 0),
(375, 1, '2026-04-01 11:45:00', '2026-04-01 11:59:59', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-04-01 11:35:40', '2026-04-01 11:35:40', 0),
(376, 1, '2026-04-01 12:00:00', '2026-04-01 15:59:59', 'Convert planner ScheduleItem code to remove GuiScheduleItem ', 0, 'Starbucks', '2026-04-01 11:37:34', '2026-04-01 11:37:34', 0),
(377, 1, '2026-04-01 16:00:00', '2026-04-01 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-01 11:38:23', '2026-04-01 11:38:23', 0),
(378, 1, '2026-04-01 17:00:00', '2026-04-01 17:59:59', 'Slice chicken, marinate chicken', 1, 'Home', '2026-04-01 11:39:57', '2026-04-01 11:39:57', 0),
(379, 1, '2026-04-01 18:00:00', '2026-04-01 18:59:59', 'Pay rent online', 1, 'Home', '2026-04-01 11:40:35', '2026-04-01 11:40:35', 0),
(380, 1, '2026-04-04 00:30:00', '2026-04-04 01:44:59', 'Zoom Sedar call with family ', 1, 'Home', '2026-04-01 11:49:04', '2026-04-01 11:49:04', 0),
(381, 1, '2026-04-04 02:00:00', '2026-04-04 02:59:59', 'Cook dinner ', 1, 'Home', '2026-04-01 11:50:33', '2026-04-01 11:50:33', 0),
(382, 1, '2026-04-03 21:00:00', '2026-04-03 21:59:59', 'Prepare Sedar plate.', 1, 'Home', '2026-04-01 11:51:58', '2026-04-01 11:51:58', 0),
(383, 1, '2026-04-08 16:00:00', '2026-04-08 16:59:59', 'Pedicure and massage', 1, 'Lovely Nails', '2026-04-01 11:54:30', '2026-04-01 11:54:30', 0),
(384, 1, '2026-04-08 21:30:00', '2026-04-08 22:29:59', 'Dr. Cinman ', 1, 'Tower Urology\n8635 West 3rd St.', '2026-04-01 11:56:26', '2026-04-01 11:56:26', 0),
(385, 1, '2026-04-02 10:45:00', '2026-04-02 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-02 11:17:58', '2026-04-02 11:17:58', 0),
(386, 1, '2026-04-02 11:00:00', '2026-04-02 11:14:59', 'Coffee at Starbucks say hello to everyone ', 1, 'Starbucks', '2026-04-02 11:18:42', '2026-04-02 11:18:42', 0),
(387, 1, '2026-04-02 11:15:00', '2026-04-02 11:45:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-02 11:19:25', '2026-04-02 11:19:25', 0),
(388, 1, '2026-04-02 11:45:00', '2026-04-02 11:59:59', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-04-02 11:20:12', '2026-04-02 11:20:12', 0),
(389, 1, '2026-04-02 12:00:00', '2026-04-02 15:59:59', 'Finish removing all GuiModels ', 0, 'Starbucks', '2026-04-02 11:21:28', '2026-04-02 11:21:28', 0),
(390, 1, '2026-04-02 16:00:00', '2026-04-02 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-02 11:22:06', '2026-04-02 11:22:06', 0),
(391, 1, '2026-04-02 17:00:00', '2026-04-02 17:59:59', 'Marinate all the chicken in preparation for freezing', 1, 'Home', '2026-04-02 11:22:50', '2026-04-02 11:22:50', 0),
(392, 1, '2026-04-02 21:00:00', '2026-04-02 21:59:59', 'Lunch and  Science Fiction', 1, 'Home', '2026-04-02 15:19:03', '2026-04-02 15:21:57', 0),
(393, 1, '2026-04-03 02:00:00', '2026-04-03 02:59:59', 'Cook dinner ', 1, 'Home', '2026-04-02 15:21:18', '2026-04-02 15:21:18', 0),
(394, 1, '2026-04-03 11:45:00', '2026-04-03 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-02 15:23:02', '2026-04-03 12:19:16', 0),
(395, 1, '2026-04-03 12:00:00', '2026-04-03 12:14:59', 'Coffee at Starbucks say hello to everyone ', 1, 'Starbucks', '2026-04-03 12:20:03', '2026-04-03 12:20:03', 0),
(396, 1, '2026-04-03 12:15:00', '2026-04-03 12:29:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-03 12:20:48', '2026-04-03 12:20:48', 0),
(397, 1, '2026-04-03 12:30:00', '2026-04-03 12:44:59', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-04-03 12:21:35', '2026-04-03 12:21:35', 0),
(398, 1, '2026-04-03 12:45:00', '2026-04-03 15:59:59', 'Reimplement schedule editor replace the current title look up with one that uses a completer', 0, 'Starbucks', '2026-04-03 12:24:10', '2026-04-03 12:24:10', 0),
(399, 1, '2026-04-03 16:00:00', '2026-04-03 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-03 12:25:08', '2026-04-03 12:25:08', 0),
(400, 1, '2026-04-03 17:00:00', '2026-04-03 17:59:59', 'Freeze chicken!!!', 1, 'Home', '2026-04-03 12:27:16', '2026-04-03 12:27:16', 0),
(401, 1, '2026-04-03 18:00:00', '2026-04-03 18:59:59', 'Testing line length changes', 0, 'Starbucks', '2026-04-03 14:22:00', '2026-04-03 14:22:00', 0),
(402, 1, '2026-04-04 11:00:00', '2026-04-04 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-03 15:53:21', '2026-04-04 11:39:02', 0),
(403, 1, '2026-04-04 11:15:00', '2026-04-04 11:29:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-04-04 11:39:57', '2026-04-04 11:39:57', 0),
(404, 1, '2026-04-04 11:30:00', '2026-04-04 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-04 11:40:34', '2026-04-04 11:40:34', 0),
(405, 1, '2026-04-04 12:00:00', '2026-04-04 12:14:59', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-04-04 11:41:05', '2026-04-04 11:41:22', 0),
(406, 1, '2026-04-04 12:15:00', '2026-04-04 15:59:59', 'Reimplement schedule editor replace the current title look up with one that uses a completer', 0, 'Starbucks', '2026-04-04 11:43:50', '2026-04-04 11:43:50', 0),
(407, 1, '2026-04-04 16:00:00', '2026-04-04 16:59:59', 'Breakfast, science fiction', 1, 'Home', '2026-04-04 11:44:39', '2026-04-04 11:44:39', 0),
(408, 1, '2026-04-04 17:00:00', '2026-04-04 17:59:59', 'Freeze chicken!!!', 1, 'Home', '2026-04-04 11:45:32', '2026-04-04 11:45:32', 0),
(409, 1, '2026-04-04 21:00:00', '2026-04-04 21:59:59', 'Lunch and science fiction ', 1, 'Home', '2026-04-04 13:05:34', '2026-04-04 13:05:34', 0),
(410, 1, '2026-04-05 02:00:00', '2026-04-05 02:59:59', 'Dinner, Pea soup!', 1, 'Norm\'s South Torrance', '2026-04-04 14:30:46', '2026-04-04 14:30:46', 0),
(411, 1, '2026-04-05 11:15:00', '2026-04-05 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-04 14:33:40', '2026-04-04 14:33:40', 0),
(412, 1, '2026-04-05 11:30:00', '2026-04-05 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-04 14:34:48', '2026-04-04 14:34:48', 0),
(413, 1, '2026-04-05 11:45:00', '2026-04-05 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-04 14:39:39', '2026-04-04 14:39:39', 0),
(414, 1, '2026-04-05 12:00:00', '2026-04-05 12:14:59', 'Code Review Moderator Oversight', 1, 'Starbucks', '2026-04-04 14:40:30', '2026-04-04 14:40:30', 0),
(415, 1, '2026-04-05 12:15:00', '2026-04-05 15:59:59', 'Debug add schedule item. Make sure all pointers to QWidgets on the stack are properly initialized.', 0, 'Starbucks', '2026-04-04 14:41:31', '2026-04-05 15:06:16', 0),
(416, 1, '2026-04-05 16:00:00', '2026-04-05 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-04 14:42:06', '2026-04-04 14:42:06', 0),
(417, 1, '2026-04-15 19:00:00', '2026-04-15 19:59:59', 'Change CPAP mask and filter', 1, 'Home', '2026-04-04 15:21:10', '2026-04-04 15:21:10', 0),
(418, 1, '2026-04-05 11:00:00', '2026-04-05 11:14:59', 'Start diswash ', 1, 'Home', '2026-04-05 12:03:56', '2026-04-05 12:03:56', 0),
(419, 1, '2026-04-05 18:00:00', '2026-04-05 18:59:59', 'Make Matzo balls', 1, 'Home', '2026-04-05 12:05:18', '2026-04-05 12:05:18', 0),
(420, 1, '2026-04-18 13:10:14', '2026-04-18 14:10:13', 'Another test of the add schedule item change date ', 1, 'Starbucks', '2026-04-05 13:10:59', '2026-04-05 13:10:59', 0),
(421, 1, '2026-05-15 19:00:00', '2026-05-15 19:14:00', 'Change CPAP mask and filter', 1, 'Home', '2026-04-05 13:25:20', '2026-04-05 13:25:20', 0),
(422, 1, '2026-06-15 19:00:00', '2026-06-15 19:14:00', 'Change CPAP mask and filter', 1, 'Home', '2026-04-05 13:26:15', '2026-04-05 13:26:15', 0),
(423, 1, '2026-04-06 10:45:00', '2026-04-06 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-06 11:53:37', '2026-04-06 11:53:37', 0),
(424, 1, '2026-04-06 11:00:00', '2026-04-06 11:14:59', 'Coffee at Starbucks say hello to everyone ', 1, 'Starbucks', '2026-04-06 11:54:30', '2026-04-06 11:54:30', 0),
(425, 1, '2026-04-06 11:30:00', '2026-04-06 11:44:59', 'Code Review Moderator Oversight', 0, 'Starbucks', '2026-04-06 11:55:37', '2026-04-06 11:56:32', 0),
(426, 1, '2026-04-06 12:00:00', '2026-04-06 12:14:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-06 11:56:28', '2026-04-06 11:56:28', 0),
(427, 1, '2026-04-06 12:15:00', '2026-04-06 15:59:59', 'Fix build to run without UI files.', 0, 'Starbucks', '2026-04-06 11:57:58', '2026-04-06 11:57:58', 0),
(428, 1, '2026-04-07 10:45:00', '2026-04-07 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-07 11:25:36', '2026-04-07 11:25:36', 0),
(429, 1, '2026-04-07 11:00:00', '2026-04-07 11:14:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-04-07 11:26:32', '2026-04-07 11:26:32', 0),
(430, 1, '2026-04-07 11:15:00', '2026-04-07 11:44:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-07 11:27:18', '2026-04-07 11:27:18', 0),
(431, 1, '2026-04-07 11:45:00', '2026-04-07 11:59:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-07 11:27:50', '2026-04-07 11:28:12', 0),
(432, 1, '2026-04-07 12:15:00', '2026-04-07 15:59:59', 'Finish implementing the delete model object', 0, 'Starbucks', '2026-04-07 11:29:41', '2026-04-07 11:30:03', 0),
(433, 1, '2026-04-07 16:00:00', '2026-04-07 16:59:59', 'Breakfast and Science Fiction', 0, 'Home', '2026-04-07 11:30:35', '2026-04-07 11:30:35', 0),
(434, 1, '2026-04-07 17:00:00', '2026-04-07 17:59:59', 'Marinate Chicken', 1, 'Home', '2026-04-07 11:31:38', '2026-04-07 11:31:38', 0),
(435, 1, '2026-04-07 20:00:00', '2026-04-07 20:59:59', 'Lunch and science fiction ', 1, 'Home', '2026-04-07 11:32:02', '2026-04-07 11:32:02', 0),
(436, 1, '2026-04-08 01:00:00', '2026-04-08 01:59:59', 'Cook dinner ', 1, 'Home', '2026-04-07 11:32:39', '2026-04-07 11:32:39', 0),
(437, 1, '2026-04-07 18:00:00', '2026-04-07 18:59:59', 'Grocery shopping ', 1, 'Redondo Beach Ralph\'s Market', '2026-04-07 11:34:06', '2026-04-07 11:34:06', 0),
(438, 1, '2026-04-08 11:15:00', '2026-04-08 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-08 12:17:18', '2026-04-08 12:17:18', 0),
(439, 1, '2026-04-08 11:30:00', '2026-04-08 11:44:59', 'Coffee at Starbucks say hello to everyone ', 1, 'Starbucks', '2026-04-08 12:18:13', '2026-04-08 12:18:13', 0),
(440, 1, '2026-04-08 11:45:00', '2026-04-08 12:15:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-08 12:19:25', '2026-04-08 12:19:25', 0),
(441, 1, '2026-04-08 12:15:00', '2026-04-08 12:29:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-08 12:21:15', '2026-04-08 12:21:15', 0),
(442, 1, '2026-04-09 11:30:00', '2026-04-09 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-09 12:13:31', '2026-04-09 12:13:31', 0),
(443, 1, '2026-04-09 11:45:00', '2026-04-09 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-09 12:14:19', '2026-04-09 12:14:19', 0),
(444, 1, '2026-04-09 12:00:00', '2026-04-09 12:29:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-09 12:15:08', '2026-04-09 12:15:08', 0),
(445, 1, '2026-04-09 12:30:00', '2026-04-09 12:44:59', 'Code Review moderator oversight. ', 1, 'Starbucks', '2026-04-09 12:15:46', '2026-04-09 12:15:46', 0),
(446, 1, '2026-04-09 13:00:00', '2026-04-09 15:59:59', 'Finish unit and integration testing for the database portion of the planner', 0, 'Starbucks', '2026-04-09 12:18:22', '2026-04-09 12:18:22', 0),
(447, 1, '2026-04-09 16:00:00', '2026-04-09 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-09 12:19:09', '2026-04-09 12:19:09', 0),
(448, 1, '2026-04-09 17:00:00', '2026-04-09 17:59:59', 'Clean grill pan', 1, 'Home', '2026-04-09 12:20:20', '2026-04-09 12:20:20', 0),
(449, 1, '2026-04-09 18:00:00', '2026-04-09 18:59:59', 'Grocery shopping ', 1, 'Redondo Beach Ralph\'s Market', '2026-04-09 12:20:58', '2026-04-09 12:20:58', 0),
(450, 1, '2026-04-10 11:15:00', '2026-04-10 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-10 11:49:57', '2026-04-10 11:49:57', 0),
(451, 1, '2026-04-10 11:30:00', '2026-04-10 11:44:59', 'Coffee at Starbucks, say hello to everyone ', 1, 'Starbucks', '2026-04-10 11:50:46', '2026-04-10 11:50:46', 0),
(452, 1, '2026-04-10 11:45:00', '2026-04-10 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-10 11:51:34', '2026-04-10 11:51:34', 0),
(453, 1, '2026-04-10 12:00:00', '2026-04-10 12:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-10 11:52:16', '2026-04-10 11:52:16', 0),
(454, 1, '2026-04-10 12:15:00', '2026-04-10 15:59:59', 'Added integration testing for the delete / hide feature', 0, 'Starbucks', '2026-04-10 11:53:19', '2026-04-10 11:53:19', 0),
(455, 1, '2026-04-10 16:00:00', '2026-04-10 16:59:59', 'Breakfast, science fiction ', 1, 'Home', '2026-04-10 11:53:46', '2026-04-10 11:53:46', 0),
(456, 1, '2026-04-11 12:30:00', '2026-04-11 12:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-11 13:48:01', '2026-04-11 13:48:01', 0),
(457, 1, '2026-04-11 12:45:00', '2026-04-11 12:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-11 13:49:15', '2026-04-11 13:49:15', 0),
(458, 1, '2026-04-11 13:00:00', '2026-04-11 13:29:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-11 13:50:41', '2026-04-11 13:51:59', 0),
(459, 1, '2026-04-11 13:30:00', '2026-04-11 13:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-11 13:51:54', '2026-04-11 13:51:54', 0),
(460, 1, '2026-04-11 14:00:00', '2026-04-11 15:59:59', 'Debug HideTask', 0, 'Starbucks', '2026-04-11 13:53:05', '2026-04-11 13:53:05', 0),
(461, 1, '2026-04-11 16:00:00', '2026-04-11 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-11 13:53:27', '2026-04-11 13:53:27', 0),
(462, 1, '2026-04-12 11:00:00', '2026-04-12 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-12 11:21:48', '2026-04-12 11:21:48', 0),
(463, 1, '2026-04-12 11:30:00', '2026-04-12 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-12 11:22:25', '2026-04-12 11:22:25', 0),
(464, 1, '2026-04-12 11:45:00', '2026-04-12 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-12 11:23:03', '2026-04-12 11:23:03', 0),
(465, 1, '2026-04-12 17:00:00', '2026-04-12 17:59:59', 'Pay American Express bill', 1, 'Home', '2026-04-12 11:47:25', '2026-04-12 11:47:25', 0),
(466, 1, '2026-04-12 12:00:00', '2026-04-12 12:14:59', 'Code Review Moderator Oversite', 0, 'Starbucks', '2026-04-12 11:48:23', '2026-04-12 11:48:23', 0),
(467, 1, '2026-04-12 12:15:00', '2026-04-12 15:59:59', 'Debug HideTask', 0, 'Starbucks', '2026-04-12 11:49:07', '2026-04-12 11:49:07', 0),
(468, 1, '2026-04-13 11:15:00', '2026-04-13 11:29:59', 'Load dishwash, start dishwash ', 1, 'Home', '2026-04-13 12:12:22', '2026-04-13 12:12:22', 0),
(469, 1, '2026-04-13 11:30:00', '2026-04-13 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-13 12:13:24', '2026-04-13 12:13:24', 0),
(470, 1, '2026-04-13 11:45:00', '2026-04-13 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-13 12:14:22', '2026-04-13 12:14:22', 0),
(471, 1, '2026-04-13 12:00:00', '2026-04-13 12:14:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-13 12:15:00', '2026-04-13 12:17:17', 0),
(472, 1, '2026-04-13 12:30:00', '2026-04-13 15:59:59', 'Finish implementing integration testing for the Hidden / Delete feature for all tables', 0, 'Starbucks', '2026-04-13 12:16:48', '2026-04-13 12:16:48', 0),
(473, 1, '2026-04-13 12:15:00', '2026-04-13 12:29:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-13 12:17:40', '2026-04-13 12:17:53', 0),
(474, 1, '2026-04-13 16:00:00', '2026-04-13 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-13 12:18:36', '2026-04-13 12:18:36', 0),
(475, 1, '2026-04-13 17:00:00', '2026-04-13 17:59:59', 'Get an appointment with Opthomologist', 1, 'Home', '2026-04-13 12:19:51', '2026-04-13 12:19:51', 0),
(476, 1, '2026-04-13 20:00:00', '2026-04-13 20:59:59', 'Lunch and science fiction ', 1, '', '2026-04-13 12:20:18', '2026-04-13 12:20:18', 0),
(477, 1, '2026-04-14 01:00:00', '2026-04-14 01:59:59', 'Start automatic rice cooker', 1, 'Home', '2026-04-13 12:21:12', '2026-04-13 12:21:12', 0),
(478, 1, '2026-04-14 02:00:00', '2026-04-14 02:59:59', 'Dinner, science fiction', 1, '', '2026-04-13 12:21:31', '2026-04-13 12:21:31', 0),
(479, 1, '2026-04-13 18:00:00', '2026-04-13 18:59:59', 'Backup the Dell computer', 1, 'Home', '2026-04-13 12:22:24', '2026-04-13 12:22:24', 0),
(480, 1, '2026-04-19 18:00:00', '2026-04-20 01:59:59', 'Jean Hom, Erect the fruit tree enclosure', 1, 'Jean Hom\'s house', '2026-04-13 12:34:42', '2026-04-19 12:20:08', 0),
(481, 1, '2026-04-13 19:00:00', '2026-04-13 19:59:59', 'Pay American Express bill', 1, 'Home', '2026-04-13 12:44:26', '2026-04-13 12:44:26', 0),
(482, 1, '2026-04-13 21:00:00', '2026-04-13 22:59:59', 'Finish implementing integration testing for the Hidden / Delete feature for all tables', 0, 'Home', '2026-04-13 16:02:03', '2026-04-13 16:02:23', 0),
(483, 1, '2026-04-14 10:45:00', '2026-04-14 10:59:59', 'Check vital signs, log data on iPhone and in spreasheet', 1, 'Home', '2026-04-14 11:26:04', '2026-04-14 11:26:04', 0),
(484, 1, '2026-04-14 10:15:00', '2026-04-14 10:44:59', 'Wash dishes from dinner ', 1, 'Home', '2026-04-14 11:26:43', '2026-04-14 11:26:43', 0),
(485, 1, '2026-04-14 11:00:00', '2026-04-14 11:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-14 11:27:17', '2026-04-14 11:27:17', 0),
(486, 1, '2026-04-14 11:15:00', '2026-04-14 11:44:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-14 11:28:03', '2026-04-14 11:28:03', 0),
(487, 1, '2026-04-14 11:45:00', '2026-04-14 11:59:59', 'Code Review moderator oversight. ', 0, '', '2026-04-14 11:28:32', '2026-04-14 11:28:32', 0),
(488, 1, '2026-04-14 12:00:00', '2026-04-14 15:59:59', 'Finish implementing integration testing for the Hidden / Delete feature for all tables', 0, 'Starbucks', '2026-04-14 11:29:39', '2026-04-14 11:29:58', 0),
(489, 1, '2026-04-14 16:00:00', '2026-04-14 16:29:59', 'Get bananas ', 1, 'Redondo Beach Ralph\'s Market', '2026-04-14 11:30:36', '2026-04-14 11:30:36', 0),
(490, 1, '2026-04-14 17:00:00', '2026-04-14 17:59:59', 'Breakfast and Science Fiction', 1, '', '2026-04-14 11:30:58', '2026-04-14 11:30:58', 0),
(491, 1, '2026-04-15 01:00:00', '2026-04-15 01:59:59', 'Cook dinner ', 1, 'Home', '2026-04-14 11:32:21', '2026-04-14 11:32:21', 0),
(492, 1, '2026-04-15 10:45:00', '2026-04-15 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-15 11:17:31', '2026-04-15 11:17:31', 0),
(493, 1, '2026-04-15 11:00:00', '2026-04-15 11:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-15 11:18:08', '2026-04-15 11:18:08', 0),
(494, 1, '2026-04-15 11:15:00', '2026-04-15 11:29:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-15 11:18:48', '2026-04-15 11:18:48', 0),
(495, 1, '2026-04-15 11:30:00', '2026-04-15 11:59:59', 'Code Review moderator oversight. ', 1, 'Starbucks', '2026-04-15 11:19:25', '2026-04-15 11:19:25', 0),
(496, 1, '2026-04-15 12:00:00', '2026-04-15 15:59:59', 'Backup the Planner database in preparation to install hide updates', 0, 'Starbucks', '2026-04-15 11:20:59', '2026-04-15 11:20:59', 0),
(497, 1, '2026-04-15 16:00:00', '2026-04-15 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-15 11:21:38', '2026-04-15 11:21:38', 0),
(498, 1, '2026-04-16 11:15:00', '2026-04-16 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-16 11:53:14', '2026-04-16 11:53:14', 0),
(499, 1, '2026-04-16 11:30:00', '2026-04-16 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-16 11:54:05', '2026-04-16 11:54:05', 0),
(500, 1, '2026-04-16 11:45:00', '2026-04-16 12:14:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-16 11:54:41', '2026-04-16 11:54:41', 0),
(501, 1, '2026-04-16 12:15:00', '2026-04-16 12:29:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-16 11:55:10', '2026-04-16 11:55:36', 0),
(502, 1, '2026-04-16 12:30:00', '2026-04-16 15:59:59', 'Add stored procedure the planner database to replace all currently generated SQL', 0, 'Starbucks', '2026-04-16 11:57:02', '2026-04-16 11:57:02', 0),
(503, 1, '2026-04-16 16:00:00', '2026-04-16 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-16 11:57:26', '2026-04-16 11:57:31', 0),
(504, 1, '2026-04-16 17:00:00', '2026-04-16 17:59:59', 'Flip chicken marinate both sides.', 1, 'Home', '2026-04-16 11:58:12', '2026-04-16 11:58:12', 0),
(505, 1, '2026-04-16 20:00:00', '2026-04-16 20:59:59', 'Lunch and science fiction ', 1, 'Home', '2026-04-16 11:59:26', '2026-04-16 11:59:26', 0),
(506, 1, '2026-04-15 22:00:00', '2026-04-15 22:59:59', 'Slice chicken, marinate chicken', 1, 'Home', '2026-04-16 12:00:17', '2026-04-16 12:00:17', 0),
(507, 1, '2026-04-17 10:45:00', '2026-04-17 10:59:59', 'Start diswash ', 1, 'Home', '2026-04-17 11:31:29', '2026-04-17 11:31:29', 0),
(508, 1, '2026-04-17 11:00:00', '2026-04-17 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-17 11:32:04', '2026-04-17 11:32:04', 0),
(509, 1, '2026-04-17 11:15:00', '2026-04-17 11:29:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-17 11:32:59', '2026-04-17 11:32:59', 0),
(510, 1, '2026-04-17 11:30:00', '2026-04-17 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-17 11:33:30', '2026-04-17 11:33:56', 0),
(511, 1, '2026-04-17 12:00:00', '2026-04-17 12:29:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-17 11:34:32', '2026-04-17 11:34:32', 0),
(512, 1, '2026-04-17 12:00:00', '2026-04-17 15:59:59', 'Convert all insert statements to stored procedures', 0, 'Starbucks', '2026-04-17 11:36:12', '2026-04-17 11:36:12', 0),
(513, 1, '2026-04-17 16:00:00', '2026-04-17 16:14:59', 'Get bananas ', 1, '', '2026-04-17 11:36:35', '2026-04-17 11:36:50', 0),
(514, 1, '2026-04-17 16:30:00', '2026-04-17 17:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-17 11:37:34', '2026-04-17 11:37:34', 0),
(515, 1, '2026-04-16 21:00:00', '2026-04-17 00:00:59', 'Convert all insert statements to stored procedures', 0, 'Home', '2026-04-17 11:38:16', '2026-04-17 11:38:16', 0),
(516, 1, '2026-04-17 20:00:00', '2026-04-17 20:59:59', 'Lunch and science fiction ', 1, 'Home', '2026-04-17 11:41:39', '2026-04-17 11:41:39', 0),
(517, 1, '2026-04-18 10:45:00', '2026-04-18 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-18 11:31:38', '2026-04-18 11:31:38', 0),
(518, 1, '2026-04-18 11:00:00', '2026-04-18 11:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-18 11:32:23', '2026-04-18 11:32:23', 0),
(519, 1, '2026-04-18 11:15:00', '2026-04-18 11:44:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-18 11:33:15', '2026-04-18 11:33:15', 0),
(520, 1, '2026-04-18 11:45:00', '2026-04-18 11:59:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-18 11:33:53', '2026-04-18 11:33:53', 0),
(521, 1, '2026-04-18 12:00:00', '2026-04-18 15:59:59', 'Convert all insert statements to stored procedures', 0, 'Starbucks', '2026-04-18 11:34:43', '2026-04-18 11:34:43', 0),
(522, 1, '2026-04-18 16:00:00', '2026-04-18 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-18 11:35:15', '2026-04-18 11:35:15', 0),
(523, 1, '2026-04-19 12:00:00', '2026-04-19 12:29:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-19 12:21:15', '2026-04-19 12:21:15', 0),
(524, 1, '2026-04-19 11:45:00', '2026-04-19 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-19 12:22:12', '2026-04-19 12:22:12', 0),
(525, 1, '2026-04-19 15:00:00', '2026-04-19 16:29:59', 'Eat breakfast, shower, shave ', 1, 'Home', '2026-04-19 12:23:52', '2026-04-19 12:23:52', 0),
(526, 1, '2026-04-19 16:30:00', '2026-04-19 17:59:59', 'Drive to Jean Hom\'s house', 1, '', '2026-04-19 12:24:27', '2026-04-19 12:24:27', 0),
(527, 1, '2026-04-20 11:00:00', '2026-04-20 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-20 12:04:26', '2026-04-20 12:04:26', 0),
(528, 1, '2026-04-20 11:15:00', '2026-04-20 11:29:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-20 12:05:18', '2026-04-20 12:05:18', 0),
(529, 1, '2026-04-20 11:30:00', '2026-04-20 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-20 12:06:06', '2026-04-20 12:06:06', 0),
(530, 1, '2026-04-20 12:00:00', '2026-04-20 12:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-20 12:06:41', '2026-04-20 12:06:41', 0),
(531, 1, '2026-04-20 12:15:00', '2026-04-20 15:59:59', 'Add more stored procedures and test them.', 0, 'Starbucks', '2026-04-20 12:16:59', '2026-04-20 12:16:59', 0),
(532, 1, '2026-04-20 16:00:00', '2026-04-20 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-20 12:17:22', '2026-04-20 12:17:22', 0),
(533, 1, '2026-04-21 01:30:00', '2026-04-21 02:59:59', 'Cook and eat dinner', 1, 'Home', '2026-04-20 12:18:11', '2026-04-20 12:18:11', 0),
(534, 1, '2026-04-21 18:00:00', '2026-04-21 20:59:59', 'Lunch with Cliff Caplan', 1, 'Red Onion', '2026-04-21 12:42:27', '2026-04-21 12:42:27', 0),
(535, 1, '2026-04-22 17:00:00', '2026-04-22 17:59:59', 'Dr Hoffman', 1, '9090 Wilshire Blcd', '2026-04-21 12:43:08', '2026-04-21 12:43:08', 0),
(536, 1, '2026-04-22 11:00:00', '2026-04-22 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-21 12:45:36', '2026-04-22 13:05:44', 0),
(537, 1, '2026-04-22 12:00:00', '2026-04-22 12:14:59', 'Because of lunch with cliff grab some donuts for an early breakfast', 1, 'Moon Donuts', '2026-04-21 12:47:19', '2026-04-21 12:47:19', 0),
(538, 1, '2026-04-22 11:15:00', '2026-04-22 11:29:59', 'Coffee at Starbcucks, say hello to people. ', 1, 'Starbucks', '2026-04-21 12:48:09', '2026-04-22 13:06:50', 0),
(539, 1, '2026-04-22 11:30:00', '2026-04-22 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-21 12:49:20', '2026-04-22 13:06:31', 0),
(540, 1, '2026-04-22 12:00:00', '2026-04-22 12:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-21 12:49:58', '2026-04-22 13:07:15', 0),
(541, 1, '2026-04-21 11:45:00', '2026-04-21 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-21 12:51:12', '2026-04-21 12:51:12', 0),
(542, 1, '2026-04-21 12:00:00', '2026-04-21 12:14:59', 'Because of lunch with cliff grab some donuts for an early breakfast', 1, 'Moon Donuts', '2026-04-21 12:51:50', '2026-04-21 12:51:50', 0),
(543, 1, '2026-04-21 12:15:00', '2026-04-21 12:30:59', 'Coffee at Starbcucks, say hello to people. ', 1, 'Starbucks', '2026-04-21 12:52:18', '2026-04-21 12:52:43', 0),
(544, 1, '2026-04-21 12:30:00', '2026-04-21 12:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-21 12:53:22', '2026-04-21 12:53:22', 0),
(545, 1, '2026-04-21 13:00:00', '2026-04-21 13:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-21 12:54:35', '2026-04-21 12:54:35', 0),
(546, 1, '2026-04-21 13:15:00', '2026-04-21 15:59:59', 'Add more stored procedures and test them.', 0, 'Starbucks', '2026-04-21 12:55:58', '2026-04-21 12:55:58', 0),
(547, 1, '2026-04-21 16:00:00', '2026-04-21 16:59:59', 'Shave and shower', 1, 'Home', '2026-04-21 12:56:29', '2026-04-21 12:56:29', 0),
(548, 1, '2026-04-22 12:15:00', '2026-04-22 14:29:59', 'Add more stored procedures and test them.', 0, 'Starbucks', '2026-04-22 13:08:07', '2026-04-22 13:08:07', 0),
(549, 1, '2026-04-22 14:30:00', '2026-04-22 15:29:59', 'Shave and shower, clean cloths', 1, 'Home', '2026-04-22 13:08:48', '2026-04-22 13:08:48', 0),
(550, 1, '2026-04-22 15:30:00', '2026-04-22 16:59:59', 'Drive to 9090 Wilshire Blvd.', 1, '', '2026-04-22 13:09:39', '2026-04-22 13:09:39', 0),
(551, 1, '2026-04-22 18:00:00', '2026-04-22 19:29:59', 'Lunch, science fiction', 1, '', '2026-04-22 13:10:48', '2026-04-22 13:10:48', 0),
(552, 1, '2026-04-23 11:00:00', '2026-04-23 11:14:59', 'Load dishwash, start dishwash ', 1, 'Home', '2026-04-23 11:52:13', '2026-04-23 11:52:13', 0),
(553, 1, '2026-04-23 11:15:00', '2026-04-23 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-23 11:52:52', '2026-04-23 11:52:52', 0),
(554, 1, '2026-04-23 11:30:00', '2026-04-23 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-23 11:53:35', '2026-04-23 11:53:35', 0),
(555, 1, '2026-04-23 11:45:00', '2026-04-23 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-23 11:54:17', '2026-04-23 11:54:17', 0),
(556, 1, '2026-04-23 12:00:00', '2026-04-23 12:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-23 11:55:34', '2026-04-23 11:55:34', 0),
(557, 1, '2026-04-23 12:15:00', '2026-04-23 15:59:59', 'Add calls to the stored procedures for insert to the models', 0, 'Starbucks', '2026-04-23 11:56:36', '2026-04-23 11:56:36', 0),
(558, 1, '2026-04-23 16:00:00', '2026-04-23 16:29:59', 'Get bananas ', 1, 'Ralph\'s market', '2026-04-23 11:57:12', '2026-04-23 11:57:12', 0),
(559, 1, '2026-04-23 16:30:00', '2026-04-23 17:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-23 11:57:49', '2026-04-23 11:57:49', 0),
(560, 1, '2026-04-24 11:45:00', '2026-04-24 11:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-24 12:22:01', '2026-04-24 12:22:01', 0),
(561, 1, '2026-04-24 12:14:00', '2026-04-24 12:29:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-24 12:22:56', '2026-04-24 12:22:56', 0),
(562, 1, '2026-04-24 12:00:00', '2026-04-24 12:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-24 12:23:42', '2026-04-24 12:23:42', 0),
(563, 1, '2026-04-24 12:30:00', '2026-04-24 12:44:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-24 12:24:45', '2026-04-24 12:24:45', 0),
(564, 1, '2026-04-24 12:45:00', '2026-04-24 15:59:59', 'Add more stored procedures and test them.', 0, 'Starbucks', '2026-04-24 12:26:21', '2026-04-24 12:26:21', 0),
(565, 1, '2026-04-24 16:00:00', '2026-04-24 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-24 12:26:43', '2026-04-24 12:26:43', 0),
(566, 1, '2026-04-25 11:15:00', '2026-04-25 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-25 12:09:30', '2026-04-25 12:09:30', 0),
(567, 1, '2026-04-25 11:30:00', '2026-04-25 11:44:59', 'Left phone at home, had to get it.', 1, '', '2026-04-25 12:10:15', '2026-04-25 12:10:15', 0),
(568, 1, '2026-04-25 11:45:00', '2026-04-25 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-25 12:11:01', '2026-04-25 12:11:01', 0),
(569, 1, '2026-04-25 12:00:00', '2026-04-25 12:14:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-25 12:11:30', '2026-04-25 12:11:30', 0),
(570, 1, '2026-04-25 12:15:00', '2026-04-25 12:29:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-25 12:12:16', '2026-04-25 12:12:16', 0),
(571, 1, '2026-04-25 12:30:00', '2026-04-25 15:59:59', 'Add more stored procedures and test them.', 0, 'Starbucks', '2026-04-25 12:13:03', '2026-04-25 12:13:03', 0),
(572, 1, '2026-04-25 16:00:00', '2026-04-25 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-25 12:13:29', '2026-04-25 12:13:29', 0),
(573, 1, '2026-04-26 02:00:00', '2026-04-26 02:59:59', 'Dinner, Pea soup!', 1, 'Norm\'s South Torrance', '2026-04-25 12:13:58', '2026-04-25 12:13:58', 0),
(574, 1, '2026-04-24 22:00:00', '2026-04-24 23:59:59', 'Add more stored procedures and test them.', 0, 'Home', '2026-04-25 12:14:40', '2026-04-25 12:14:40', 0),
(575, 1, '2026-04-25 02:00:00', '2026-04-25 02:59:59', 'Cook and eat dinner', 1, 'Home', '2026-04-25 12:15:15', '2026-04-25 12:15:15', 0),
(576, 1, '2026-04-26 11:30:00', '2026-04-26 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-26 12:14:16', '2026-04-26 12:14:16', 0),
(577, 1, '2026-04-26 11:45:00', '2026-04-26 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-26 12:15:12', '2026-04-26 12:15:12', 0),
(578, 1, '2026-04-26 12:00:00', '2026-04-26 12:14:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-26 12:16:03', '2026-04-26 12:16:03', 0),
(579, 1, '2026-04-26 12:15:00', '2026-04-26 12:29:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-04-26 12:16:51', '2026-04-26 12:16:51', 0),
(580, 1, '2026-04-26 12:30:00', '2026-04-26 15:59:59', 'Add more stored procedures and test them.', 0, 'Starbucks', '2026-04-26 12:17:47', '2026-04-26 12:17:47', 0),
(581, 1, '2026-04-26 16:00:00', '2026-04-26 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-26 12:18:07', '2026-04-26 12:18:07', 0),
(582, 1, '2026-04-26 17:00:00', '2026-04-26 17:59:59', 'Grocery shopping ', 1, 'Redondo Beach Ralph\'s Market', '2026-04-26 12:18:38', '2026-04-26 12:18:38', 0),
(583, 1, '2026-04-27 01:00:00', '2026-04-27 01:59:59', 'Prepare and cook rice', 1, 'Home', '2026-04-26 12:20:15', '2026-04-26 12:20:15', 0),
(584, 1, '2026-04-27 02:00:00', '2026-04-27 02:59:59', 'Cook and eat dinner', 1, 'Home', '2026-04-26 12:20:49', '2026-04-26 12:20:49', 0),
(585, 1, '2026-04-27 11:15:08', '2026-04-27 11:29:07', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-27 11:54:43', '2026-04-27 11:54:43', 0),
(586, 1, '2026-04-27 11:30:00', '2026-04-27 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-27 11:55:48', '2026-04-27 11:55:48', 0),
(587, 1, '2026-04-27 11:45:00', '2026-04-27 12:15:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-27 11:56:45', '2026-04-27 11:56:45', 0),
(588, 1, '2026-04-27 12:15:00', '2026-04-27 12:30:59', 'Code Review moderator oversight. ', 1, 'Starbucks', '2026-04-27 11:57:59', '2026-04-27 11:57:59', 0),
(589, 1, '2026-04-27 12:30:00', '2026-04-27 15:59:59', 'Add more stored procedures and test them.', 1, 'Starbucks', '2026-04-27 11:58:51', '2026-04-27 11:58:51', 0),
(590, 1, '2026-04-27 16:00:00', '2026-04-27 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-04-27 11:59:22', '2026-04-27 11:59:22', 0),
(591, 1, '2026-04-27 22:00:00', '2026-04-27 22:59:59', 'Food preparation, cut chicken into 1/3 pound fillets. Chop vegetables ', 1, 'Home', '2026-04-27 12:00:06', '2026-04-27 12:00:06', 0),
(592, 1, '2026-04-27 20:00:00', '2026-04-27 20:59:59', 'Lunch and science fiction ', 1, 'Home', '2026-04-27 12:00:43', '2026-04-27 12:00:43', 0),
(593, 1, '2026-04-27 17:00:00', '2026-04-27 17:59:59', 'Call State Farm, check personal liability coverage', 1, 'Home', '2026-04-27 12:01:52', '2026-04-27 12:01:52', 0),
(594, 1, '2026-04-27 18:00:00', '2026-04-27 19:59:59', 'Add more stored procedures and test them.', 0, 'Home', '2026-04-27 12:03:07', '2026-04-27 12:03:07', 0),
(595, 1, '2026-04-28 00:00:00', '2026-04-28 00:59:59', 'Start automatic rice cooker', 1, 'Home', '2026-04-27 12:03:40', '2026-04-27 12:03:40', 0),
(596, 1, '2026-04-27 21:00:00', '2026-04-27 21:59:59', 'Go to Del Amo Mall, check out the Apple store', 1, 'Del Amo Mall', '2026-04-27 12:05:24', '2026-04-27 12:05:24', 0),
(597, 1, '2026-04-28 10:45:00', '2026-04-28 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-04-28 11:18:37', '2026-04-28 11:18:37', 0),
(598, 1, '2026-04-28 11:00:00', '2026-04-28 11:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-04-28 11:19:15', '2026-04-28 11:19:15', 0),
(599, 1, '2026-04-28 11:15:00', '2026-04-28 11:30:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-04-28 11:19:58', '2026-04-28 11:19:58', 0),
(600, 1, '2026-04-28 11:30:00', '2026-04-28 11:44:59', 'Code Review moderator oversight. ', 1, 'Starbucks', '2026-04-28 11:20:48', '2026-04-28 11:20:48', 0),
(601, 1, '2026-04-28 12:00:00', '2026-04-28 15:59:59', 'Add more stored procedures and test them.', 1, 'Starbucks', '2026-04-28 11:21:32', '2026-04-28 11:21:32', 0),
(602, 1, '2026-04-28 16:00:00', '2026-04-28 16:29:59', 'Get bananas ', 1, '', '2026-04-28 11:22:13', '2026-04-28 11:22:27', 0),
(603, 1, '2026-04-28 16:30:00', '2026-04-28 17:59:59', 'Breakfast and Science Fiction', 1, '', '2026-04-28 11:23:00', '2026-04-28 11:23:00', 0),
(604, 1, '2026-04-28 20:00:00', '2026-04-28 20:59:59', 'Go to Del Amo Mall, check out the Apple store', 1, 'Del Amo Mall', '2026-04-28 11:23:55', '2026-04-28 11:23:55', 0),
(605, 1, '2026-04-28 18:00:00', '2026-04-28 18:59:59', 'Wash pots and pans from last nights dinner ', 1, 'Home', '2026-04-28 11:24:25', '2026-04-28 11:24:25', 0),
(606, 1, '2026-05-01 10:45:00', '2026-05-01 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-01 11:28:13', '2026-05-01 11:28:13', 0),
(607, 1, '2026-05-01 11:00:00', '2026-05-01 11:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-01 11:29:19', '2026-05-01 11:29:19', 0),
(608, 1, '2026-05-01 11:15:00', '2026-05-01 11:45:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-01 11:29:57', '2026-05-01 11:29:57', 0),
(609, 1, '2026-05-01 11:45:00', '2026-05-01 11:59:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-01 11:30:32', '2026-05-01 11:30:32', 0),
(610, 1, '2026-05-01 12:00:00', '2026-05-01 15:59:59', 'Add more stored procedures and test them.', 0, 'Starbucks', '2026-05-01 11:31:18', '2026-05-01 11:31:18', 0),
(611, 1, '2026-05-01 16:00:00', '2026-05-01 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-01 11:31:42', '2026-05-01 11:31:42', 0),
(612, 1, '2026-05-02 00:00:00', '2026-05-02 00:59:59', 'Prepare and cook rice', 1, 'Home', '2026-05-01 11:33:11', '2026-05-01 11:33:11', 0),
(613, 1, '2026-05-05 14:00:00', '2026-05-05 14:29:59', 'Grocery shopping ', 1, '', '2026-05-05 11:50:42', '2026-05-05 11:50:42', 0),
(614, 1, '2026-05-05 11:15:00', '2026-05-05 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-05 11:51:49', '2026-05-05 11:51:49', 0),
(615, 1, '2026-05-05 14:30:00', '2026-05-05 14:59:59', 'Vacume car', 1, 'Self car wash', '2026-05-05 11:53:04', '2026-05-05 11:53:04', 0),
(616, 1, '2026-05-05 11:30:00', '2026-05-05 11:44:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-05 11:54:06', '2026-05-05 11:54:06', 0),
(617, 1, '2026-05-05 11:45:00', '2026-05-05 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-05 11:54:55', '2026-05-05 11:54:55', 0),
(618, 1, '2026-05-05 12:00:00', '2026-05-05 13:59:59', 'Add more stored procedures and test them.', 0, 'Starbucks', '2026-05-05 11:56:25', '2026-05-05 11:56:25', 0),
(619, 1, '2026-05-05 15:30:00', '2026-05-05 16:29:59', 'Drop car off at Redondo Beach Foreign Auto', 1, '', '2026-05-05 11:58:38', '2026-05-05 11:58:38', 0),
(620, 1, '2026-05-09 11:00:00', '2026-05-09 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-09 11:31:54', '2026-05-09 11:31:54', 0),
(621, 1, '2026-05-09 11:15:00', '2026-05-09 11:29:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-09 11:32:43', '2026-05-09 11:32:43', 0),
(622, 1, '2026-05-09 11:30:00', '2026-05-09 11:44:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-09 11:33:28', '2026-05-09 11:33:28', 0),
(623, 1, '2026-05-09 11:45:00', '2026-05-09 11:59:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-09 11:34:39', '2026-05-09 11:34:39', 0),
(624, 1, '2026-05-09 12:00:00', '2026-05-09 15:59:59', 'Figure out how to self test Query Processor modules', 0, 'Starbucks', '2026-05-09 11:36:10', '2026-05-09 11:36:10', 0),
(625, 1, '2026-05-09 16:00:00', '2026-05-09 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-09 11:36:57', '2026-05-09 11:36:57', 0),
(626, 1, '2026-05-08 23:00:00', '2026-05-08 23:59:59', 'Pick up car', 1, 'Redondo Foreign Auto', '2026-05-09 11:39:16', '2026-05-09 11:39:16', 0),
(627, 1, '2026-05-20 18:30:00', '2026-05-20 18:59:59', 'Dr. Aftergood', 1, '99 N. La Cienaga', '2026-05-20 15:42:02', '2026-05-20 15:42:02', 0),
(628, 1, '2026-05-20 10:45:00', '2026-05-20 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-20 15:42:50', '2026-05-20 15:42:50', 0),
(629, 1, '2026-05-20 11:00:00', '2026-05-20 11:14:59', 'Coffee, say hellp to everyone ', 1, 'Starbucks', '2026-05-20 15:43:29', '2026-05-20 15:43:29', 0),
(630, 1, '2026-05-20 11:15:00', '2026-05-20 11:45:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-20 15:44:13', '2026-05-20 15:44:13', 0);
INSERT INTO `UserScheduleItem` (`idUserScheduleItem`, `UserID`, `StartDateTime`, `EndDateTime`, `Title`, `Personal`, `Location`, `CreatedTS`, `LastUpdateTS`, `Hidden`) VALUES
(631, 1, '2026-05-20 11:45:00', '2026-05-20 14:14:59', 'Get all Unit tests working with new architecture.', 0, 'Starbucks', '2026-05-20 15:45:23', '2026-05-20 15:45:23', 0),
(632, 1, '2026-05-20 14:30:00', '2026-05-20 15:14:59', 'Get gas vacuum car', 1, '', '2026-05-20 15:46:23', '2026-05-20 15:46:23', 0),
(633, 1, '2026-05-20 17:00:00', '2026-05-20 18:29:59', 'Drive to 99 N. La Cienega', 1, '', '2026-05-20 15:47:10', '2026-05-20 15:47:10', 0),
(634, 1, '2026-05-20 16:00:00', '2026-05-20 16:59:59', 'Shave and shower', 1, 'Home', '2026-05-20 15:47:28', '2026-05-20 15:47:28', 0),
(635, 1, '2026-05-20 19:00:00', '2026-05-20 19:59:59', 'Lunch and science fiction ', 1, '', '2026-05-20 15:47:49', '2026-05-20 15:47:49', 0),
(636, 1, '2026-05-22 10:45:00', '2026-05-22 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-22 11:33:23', '2026-05-22 11:33:23', 0),
(637, 1, '2026-05-22 11:00:00', '2026-05-22 11:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-22 11:34:26', '2026-05-22 11:34:26', 0),
(638, 1, '2026-05-22 11:15:00', '2026-05-22 11:45:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-22 11:35:13', '2026-05-22 11:35:13', 0),
(639, 1, '2026-05-22 11:45:00', '2026-05-22 11:59:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-22 11:35:49', '2026-05-22 11:35:49', 0),
(640, 1, '2026-05-22 12:00:00', '2026-05-22 15:59:59', 'Finish implemeneting stored procedures', 0, 'Starbucks', '2026-05-22 11:37:05', '2026-05-22 11:37:05', 0),
(641, 1, '2026-05-22 16:00:00', '2026-05-22 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-22 11:37:27', '2026-05-22 11:37:27', 0),
(642, 1, '2026-05-22 20:00:00', '2026-05-22 20:59:59', 'Lunch and science fiction ', 1, 'Home', '2026-05-22 11:38:30', '2026-05-22 11:38:30', 0),
(643, 1, '2026-05-23 01:00:00', '2026-05-23 01:59:59', 'Cook and eat dinner', 1, 'Home', '2026-05-22 11:38:55', '2026-05-22 11:38:55', 0),
(644, 1, '2026-05-22 18:00:00', '2026-05-22 18:59:59', 'Backup the Planner database in preparation to install new database architecture', 0, 'Home', '2026-05-22 11:40:17', '2026-05-22 11:40:17', 0),
(645, 1, '2026-05-23 12:00:00', '2026-05-23 12:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-23 13:09:14', '2026-05-23 13:09:14', 0),
(646, 1, '2026-05-23 12:15:00', '2026-05-23 12:29:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-23 13:10:18', '2026-05-23 13:10:18', 0),
(647, 1, '2026-05-23 12:30:00', '2026-05-23 12:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-23 13:11:14', '2026-05-23 13:11:14', 0),
(648, 1, '2026-05-23 13:00:00', '2026-05-23 15:59:59', 'Backup the Planner database in preparation to install new database architecture', 0, 'Starbucks', '2026-05-23 13:12:05', '2026-05-23 13:12:05', 0),
(649, 1, '2026-05-24 11:00:00', '2026-05-24 11:15:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-24 11:54:15', '2026-05-24 11:54:15', 0),
(650, 1, '2026-05-24 11:30:00', '2026-05-24 11:44:59', 'Coffee, chat with people ', 1, 'Starbucks', '2026-05-24 11:54:54', '2026-05-24 11:54:54', 0),
(651, 1, '2026-05-24 11:45:00', '2026-05-24 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-24 11:55:36', '2026-05-24 11:55:36', 0),
(652, 1, '2026-05-24 12:00:00', '2026-05-24 12:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-24 11:57:07', '2026-05-24 11:57:07', 0),
(653, 1, '2026-05-24 12:15:00', '2026-05-24 15:59:59', 'Merge model code for new database architecture into the GUI development', 0, 'Starbucks', '2026-05-24 11:58:08', '2026-05-24 11:58:08', 0),
(654, 1, '2026-05-24 16:00:00', '2026-05-24 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-24 11:58:37', '2026-05-24 11:58:37', 0),
(655, 1, '2026-05-24 17:00:00', '2026-05-24 17:14:59', 'Send a text to Eitan', 1, 'Home', '2026-05-24 11:59:38', '2026-05-24 11:59:38', 0),
(656, 1, '2026-05-24 20:00:00', '2026-05-24 20:59:59', 'Lunch and science fiction ', 1, 'Home', '2026-05-24 12:00:27', '2026-05-24 12:00:27', 0),
(657, 1, '2026-05-25 01:00:00', '2026-05-25 01:59:59', 'Cook and eat dinner', 1, 'Home', '2026-05-24 12:00:55', '2026-05-24 12:00:55', 0),
(658, 1, '2026-05-24 18:00:00', '2026-05-24 19:59:59', 'Merge model code for new database architecture into the GUI development', 0, 'Home', '2026-05-24 12:01:35', '2026-05-24 12:01:35', 0),
(659, 1, '2026-05-24 21:00:00', '2026-05-24 23:59:59', 'Put together a resume for the guy on LinkedIn', 1, 'Home', '2026-05-24 12:02:35', '2026-05-24 12:02:35', 0),
(660, 1, '2026-05-25 11:15:00', '2026-05-25 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-25 11:56:18', '2026-05-25 11:56:18', 0),
(661, 1, '2026-05-25 11:30:30', '2026-05-25 11:44:29', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-25 11:57:30', '2026-05-25 11:57:30', 0),
(662, 1, '2026-05-25 11:45:34', '2026-05-25 11:59:33', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-25 11:58:10', '2026-05-25 11:58:10', 0),
(663, 1, '2026-05-25 12:00:00', '2026-05-25 12:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-25 11:59:23', '2026-05-25 11:59:23', 0),
(664, 1, '2026-05-25 12:15:00', '2026-05-25 15:59:59', 'Final save of original database, then upgrade database and restore from saved data', 0, 'Starbucks', '2026-05-25 12:01:23', '2026-05-25 12:01:23', 0),
(665, 1, '2026-05-25 16:00:00', '2026-05-25 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-25 12:01:56', '2026-05-25 12:01:56', 0),
(666, 1, '2026-05-25 20:00:00', '2026-05-25 20:59:59', 'Lunch and science fiction ', 1, 'Home', '2026-05-25 12:02:15', '2026-05-25 12:02:15', 0),
(667, 1, '2026-05-26 00:00:00', '2026-05-26 00:59:59', 'Prepare and cook rice', 1, 'Home', '2026-05-25 12:03:01', '2026-05-25 12:03:01', 0),
(668, 1, '2026-05-26 01:00:00', '2026-05-26 01:59:59', 'Cook and eat dinner', 1, 'Home', '2026-05-25 12:03:20', '2026-05-25 12:03:20', 0),
(669, 1, '2026-05-25 17:00:00', '2026-05-25 17:59:59', 'Update resume and send it to Jayaprakash', 1, 'Home', '2026-05-25 12:13:07', '2026-05-25 12:13:07', 0),
(670, 1, '2026-05-26 11:00:00', '2026-05-26 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-26 11:39:08', '2026-05-26 11:39:08', 0),
(671, 1, '2026-05-26 11:15:00', '2026-05-26 11:29:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-26 11:40:02', '2026-05-26 11:40:02', 0),
(672, 1, '2026-05-26 11:30:00', '2026-05-26 11:44:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-26 11:40:48', '2026-05-26 11:40:48', 0),
(673, 1, '2026-05-26 11:45:00', '2026-05-26 11:59:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-26 11:41:33', '2026-05-26 11:41:33', 0),
(674, 1, '2026-05-26 16:00:00', '2026-05-26 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-26 11:41:55', '2026-05-26 11:41:55', 0),
(675, 1, '2026-05-26 17:00:00', '2026-05-26 17:59:59', 'Steve Little State Farm Renters Insurance', 1, '', '2026-05-26 11:42:36', '2026-05-26 11:42:36', 0),
(676, 1, '2026-05-26 12:00:00', '2026-05-26 15:59:59', 'Add and remove header files as indicated by clang interface of QtCreator.', 0, 'Starbucks', '2026-05-26 14:25:57', '2026-05-26 14:25:57', 0),
(677, 1, '2026-05-26 21:00:00', '2026-05-26 21:59:59', 'Lunch and  Science Fiction', 1, 'Home', '2026-05-26 14:28:41', '2026-05-26 14:28:41', 0),
(678, 1, '2026-05-27 01:00:00', '2026-05-27 01:59:59', 'Cook and eat dinner', 1, 'Home', '2026-05-26 14:29:03', '2026-05-26 14:29:03', 0),
(679, 1, '2026-05-27 10:45:00', '2026-05-27 11:05:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-27 11:39:26', '2026-05-27 11:39:53', 0),
(680, 1, '2026-05-27 11:15:00', '2026-05-27 11:29:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-27 11:40:27', '2026-05-27 11:40:27', 0),
(681, 1, '2026-05-27 11:30:00', '2026-05-27 11:44:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-27 11:41:03', '2026-05-27 11:41:54', 0),
(682, 1, '2026-05-27 11:45:00', '2026-05-27 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-27 11:41:49', '2026-05-27 11:41:49', 0),
(683, 1, '2026-05-27 12:00:00', '2026-05-27 15:59:59', 'Add and remove header files as indicated by clang interface of QtCreator.', 0, 'Starbucks', '2026-05-27 11:44:04', '2026-05-27 11:44:04', 0),
(684, 1, '2026-05-27 16:00:00', '2026-05-27 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-27 11:44:25', '2026-05-27 11:44:25', 0),
(685, 1, '2026-05-27 20:00:00', '2026-05-27 20:59:59', 'Lunch and  Science Fiction', 1, 'Home', '2026-05-27 11:44:55', '2026-05-27 11:44:55', 0),
(686, 1, '2026-05-28 01:00:00', '2026-05-28 01:59:59', 'Cook and eat dinner', 1, 'Home', '2026-05-27 11:45:16', '2026-05-27 11:45:16', 0),
(687, 1, '2026-05-28 11:15:00', '2026-05-28 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-28 11:52:20', '2026-05-28 11:52:20', 0),
(688, 1, '2026-05-28 11:30:00', '2026-05-28 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-28 11:53:01', '2026-05-28 11:53:01', 0),
(689, 1, '2026-05-28 11:45:00', '2026-05-28 12:14:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-28 11:53:55', '2026-05-28 11:53:55', 0),
(690, 1, '2026-05-28 12:15:00', '2026-05-28 15:59:59', 'Goal list implementation', 0, 'Starbucks', '2026-05-28 12:09:26', '2026-05-28 12:09:26', 0),
(691, 1, '2026-05-28 16:00:00', '2026-05-28 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-28 12:11:02', '2026-05-28 12:11:02', 0),
(692, 1, '2026-05-28 20:00:00', '2026-05-28 20:59:59', 'Lunch and  Science Fiction', 1, 'Home', '2026-05-28 12:11:23', '2026-05-28 12:11:23', 0),
(693, 1, '2026-05-29 01:00:00', '2026-05-29 01:59:59', 'Start rice cooker', 1, 'Home', '2026-05-28 12:12:04', '2026-05-28 12:12:04', 0),
(694, 1, '2026-05-29 02:00:00', '2026-05-29 02:59:59', 'Cook and eat dinner', 1, 'Home', '2026-05-28 12:12:25', '2026-05-28 12:12:25', 0),
(695, 1, '2026-05-28 17:00:00', '2026-05-28 17:59:59', 'Send insurance information to Steve Little', 1, 'Home', '2026-05-28 12:13:12', '2026-05-28 12:13:12', 0),
(696, 1, '2026-05-29 11:30:00', '2026-05-29 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-29 12:13:35', '2026-05-29 12:13:35', 0),
(697, 1, '2026-05-29 11:45:00', '2026-05-29 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-29 12:14:41', '2026-05-29 12:14:41', 0),
(698, 1, '2026-05-29 12:00:00', '2026-05-29 12:14:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-29 12:15:41', '2026-05-29 12:26:34', 0),
(699, 1, '2026-05-29 12:30:00', '2026-05-29 15:59:59', 'Work on separate windows for each of the models for presentation', 0, 'Starbucks', '2026-05-29 12:17:10', '2026-05-29 12:17:10', 0),
(700, 1, '2026-05-29 16:00:00', '2026-05-29 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-29 12:17:35', '2026-05-29 12:17:35', 0),
(701, 1, '2026-05-29 20:00:00', '2026-05-29 20:59:59', 'Lunch and  Science Fiction', 1, 'Home', '2026-05-29 12:18:17', '2026-05-29 12:18:17', 0),
(702, 1, '2026-05-29 17:00:00', '2026-05-29 17:59:59', 'Send insurance information to Steve Little', 1, 'Home', '2026-05-29 12:18:46', '2026-05-29 12:18:46', 0),
(703, 1, '2026-05-29 12:15:00', '2026-05-29 12:30:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-29 12:27:13', '2026-05-29 12:27:13', 0),
(704, 1, '2026-05-30 10:30:00', '2026-05-30 10:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-30 11:25:15', '2026-05-30 11:25:15', 0),
(705, 1, '2026-05-30 11:00:00', '2026-05-30 11:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-30 11:25:42', '2026-05-30 11:25:57', 0),
(706, 1, '2026-05-30 11:15:00', '2026-05-30 11:45:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-30 11:26:34', '2026-05-30 11:26:34', 0),
(707, 1, '2026-05-30 11:45:00', '2026-05-30 11:59:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-30 11:27:05', '2026-05-30 11:27:05', 0),
(708, 1, '2026-05-30 12:00:00', '2026-05-30 15:59:59', 'Work on separate windows for each of the models for presentation', 0, 'Starbucks', '2026-05-30 11:28:26', '2026-05-30 11:28:26', 0),
(709, 1, '2026-05-30 16:00:00', '2026-05-30 16:59:59', 'Grocery shopping ', 1, 'Redondo Beach Ralph\'s Market', '2026-05-30 11:29:07', '2026-05-30 11:29:07', 0),
(710, 1, '2026-05-30 17:00:00', '2026-05-30 17:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-30 11:29:29', '2026-05-30 11:29:29', 0),
(711, 1, '2026-05-30 21:00:00', '2026-05-30 21:59:59', 'Lunch and Science Fiction	', 1, 'Home', '2026-05-30 11:30:23', '2026-05-30 11:30:23', 0),
(712, 1, '2026-05-31 01:00:00', '2026-05-31 01:59:59', 'Dinner, Pea soup!', 1, 'Norm\'s South Torrance', '2026-05-30 11:30:46', '2026-05-30 11:30:46', 0),
(713, 1, '2026-05-31 11:30:00', '2026-05-31 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-05-31 11:57:40', '2026-05-31 11:57:40', 0),
(714, 1, '2026-05-31 11:45:00', '2026-05-31 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-05-31 11:58:24', '2026-05-31 11:58:24', 0),
(715, 1, '2026-05-31 12:00:00', '2026-05-31 12:14:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-05-31 11:59:07', '2026-05-31 11:59:07', 0),
(716, 1, '2026-05-31 12:15:00', '2026-05-31 12:29:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-05-31 11:59:52', '2026-05-31 11:59:52', 0),
(717, 1, '2026-05-31 12:30:00', '2026-05-31 15:59:59', 'Work on separate windows for each of the models for presentation', 0, 'Starbucks', '2026-05-31 12:01:20', '2026-05-31 12:01:20', 0),
(718, 1, '2026-05-31 16:00:00', '2026-05-31 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-05-31 12:01:44', '2026-05-31 12:01:44', 0),
(719, 1, '2026-05-31 17:00:00', '2026-05-31 17:59:59', 'Cut chicken into fillets and marinate the chicken.', 1, 'Home', '2026-05-31 12:02:09', '2026-05-31 12:02:31', 0),
(720, 1, '2026-05-31 20:00:00', '2026-05-31 20:59:59', 'Lunch and Science Fiction	 ', 1, 'Home', '2026-05-31 12:03:23', '2026-05-31 12:03:23', 0),
(721, 1, '2026-06-01 01:00:00', '2026-06-01 01:59:59', 'Start automatic rice cooker', 1, 'Home', '2026-05-31 12:04:18', '2026-05-31 12:04:18', 0),
(722, 1, '2026-06-01 02:00:00', '2026-06-01 02:59:59', 'Cook and eat dinner', 1, 'Home', '2026-05-31 12:04:48', '2026-05-31 12:04:48', 0),
(723, 1, '2026-06-01 04:00:58', '2026-06-01 04:15:57', 'Inject meds', 1, 'Home', '2026-05-31 12:05:42', '2026-05-31 12:05:42', 0),
(724, 1, '2026-05-30 20:00:00', '2026-05-30 20:59:59', 'Text Eitan', 1, 'Home', '2026-05-31 12:06:32', '2026-05-31 12:07:17', 0),
(725, 1, '2026-05-30 22:00:00', '2026-05-30 23:59:59', 'Work on separate windows for each of the models for presentation', 0, 'Home', '2026-05-31 12:07:06', '2026-05-31 12:07:12', 0),
(726, 1, '2026-05-31 00:00:00', '2026-05-31 00:59:59', 'Call Dina, check in', 1, 'Home', '2026-05-31 12:07:53', '2026-05-31 12:07:53', 0),
(727, 1, '2026-05-31 21:00:00', '2026-05-31 22:59:59', 'Work on separate windows for each of the models for presentation', 0, 'Home', '2026-05-31 12:22:38', '2026-05-31 12:22:38', 0),
(728, 1, '2026-06-02 10:30:55', '2026-06-02 10:44:54', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-06-02 11:50:36', '2026-06-02 11:50:36', 0),
(729, 1, '2026-06-02 11:00:00', '2026-06-02 11:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-06-02 11:51:19', '2026-06-02 11:51:19', 0),
(730, 1, '2026-06-02 11:15:00', '2026-06-02 15:59:59', 'Work on separate windows for each of the models for presentation', 0, 'Starbucks', '2026-06-02 11:57:50', '2026-06-02 11:57:50', 0),
(731, 1, '2026-06-03 10:30:09', '2026-06-03 10:44:08', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-06-03 11:20:54', '2026-06-03 11:20:54', 0),
(732, 1, '2026-06-03 11:00:57', '2026-06-03 11:14:56', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-06-03 11:21:34', '2026-06-03 11:21:34', 0),
(733, 1, '2026-06-03 11:15:38', '2026-06-03 11:29:37', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-06-03 11:22:12', '2026-06-03 11:22:12', 0),
(734, 1, '2026-06-03 11:30:31', '2026-06-03 11:44:30', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-06-03 11:22:59', '2026-06-03 11:22:59', 0),
(735, 1, '2026-06-03 11:45:00', '2026-06-03 15:59:59', 'Work on separate windows for each of the models for presentation', 0, 'Starbucks', '2026-06-03 11:24:43', '2026-06-03 11:24:43', 0),
(736, 1, '2026-06-03 16:00:00', '2026-06-03 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-06-03 11:25:10', '2026-06-03 11:25:10', 0),
(737, 1, '2026-06-03 20:00:00', '2026-06-03 20:59:59', 'Lunch and  Science Fiction', 1, 'Home', '2026-06-03 11:25:29', '2026-06-03 11:25:29', 0),
(738, 1, '2026-06-04 01:30:00', '2026-06-04 02:29:59', 'Cook and eat dinner', 1, 'Home', '2026-06-03 11:25:57', '2026-06-03 11:25:57', 0),
(739, 1, '2026-06-04 11:15:00', '2026-06-04 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-06-04 11:57:27', '2026-06-04 11:57:27', 0),
(740, 1, '2026-06-04 11:30:00', '2026-06-04 11:49:59', 'Coffee, say hello to everyone ', 0, 'Starbucks', '2026-06-04 11:58:22', '2026-06-04 11:58:22', 0),
(741, 1, '2026-06-04 11:45:00', '2026-06-04 12:15:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-06-04 11:59:10', '2026-06-04 11:59:10', 0),
(742, 1, '2026-06-04 12:15:00', '2026-06-04 12:29:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-06-04 12:00:53', '2026-06-04 12:00:53', 0),
(743, 1, '2026-06-04 12:30:00', '2026-06-04 15:59:59', 'Goal list implementation', 0, 'Starbucks', '2026-06-04 12:02:23', '2026-06-04 12:02:23', 0),
(744, 1, '2026-06-04 16:00:00', '2026-06-04 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-06-04 12:04:25', '2026-06-04 12:04:25', 0),
(745, 1, '2026-06-04 20:00:00', '2026-06-04 20:59:59', 'Lunch and  Science Fiction', 1, 'Home', '2026-06-04 12:04:46', '2026-06-04 12:04:46', 0),
(746, 1, '2026-06-05 01:00:00', '2026-06-05 01:59:59', 'Cook and eat dinner', 1, 'Home', '2026-06-04 12:05:14', '2026-06-04 12:05:14', 0),
(747, 1, '2026-06-04 23:30:00', '2026-06-04 23:59:59', 'Walk around the block', 1, 'Home', '2026-06-04 12:06:04', '2026-06-04 12:06:04', 0),
(748, 1, '2026-06-10 16:00:00', '2026-06-10 16:59:59', 'Pedicure and massage', 1, 'Lovely Nails', '2026-06-04 12:07:59', '2026-06-04 12:07:59', 0),
(749, 1, '2026-06-05 11:15:00', '2026-06-05 11:29:59', 'Load dishwash, start dishwash ', 1, 'Home', '2026-06-05 12:01:28', '2026-06-05 12:01:28', 0),
(750, 1, '2026-06-05 11:30:00', '2026-06-05 11:44:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-06-05 12:02:07', '2026-06-05 12:02:07', 0),
(751, 1, '2026-06-05 11:44:00', '2026-06-05 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-06-05 12:03:51', '2026-06-05 12:03:51', 0),
(752, 1, '2026-06-05 12:00:00', '2026-06-05 12:15:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-06-05 12:04:20', '2026-06-05 12:04:20', 0),
(753, 1, '2026-06-05 12:15:00', '2026-06-05 12:29:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-06-05 12:05:22', '2026-06-05 12:05:22', 0),
(754, 1, '2026-06-05 12:30:00', '2026-06-05 15:59:59', 'Finish implementing the Goal Window', 0, 'Starbucks', '2026-06-05 12:07:34', '2026-06-05 12:07:34', 0),
(755, 1, '2026-06-05 16:00:00', '2026-06-05 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-06-05 12:08:07', '2026-06-05 12:08:07', 0),
(756, 1, '2026-06-05 17:00:00', '2026-06-05 17:59:59', 'Insurance Certificate', 1, 'Home', '2026-06-05 12:08:37', '2026-06-05 12:08:37', 0),
(757, 1, '2026-06-05 20:00:00', '2026-06-05 20:59:59', 'Lunch and  Science Fiction', 0, 'Home', '2026-06-05 12:08:54', '2026-06-05 12:08:54', 0),
(758, 1, '2026-06-06 01:00:00', '2026-06-06 01:59:59', 'Cook and eat dinner', 1, 'Home', '2026-06-05 12:09:14', '2026-06-05 12:09:14', 0),
(759, 1, '2026-06-06 00:00:00', '2026-06-06 00:59:59', 'Walk around the block', 1, 'Home', '2026-06-05 12:09:31', '2026-06-05 12:09:31', 0),
(760, 1, '2026-06-06 12:00:00', '2026-06-06 12:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-06-06 12:44:37', '2026-06-06 12:44:37', 0),
(761, 1, '2026-06-06 12:15:00', '2026-06-06 12:29:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-06-06 12:45:28', '2026-06-06 12:45:28', 0),
(762, 1, '2026-06-06 12:30:00', '2026-06-06 12:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-06-06 12:46:18', '2026-06-06 12:46:18', 0),
(763, 1, '2026-06-06 13:00:00', '2026-06-06 13:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-06-06 12:46:52', '2026-06-06 12:46:52', 0),
(764, 1, '2026-06-06 13:15:00', '2026-06-06 15:59:59', 'Clean up Goal code, check in files.', 0, 'Starbucks', '2026-06-06 12:47:57', '2026-06-06 12:47:57', 0),
(765, 1, '2026-06-06 16:00:00', '2026-06-06 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-06-06 12:48:21', '2026-06-06 12:48:21', 0),
(766, 1, '2026-06-07 01:30:00', '2026-06-07 02:59:59', 'Dinner, Pea soup!', 1, 'Norm\'s South Torrance', '2026-06-06 12:49:11', '2026-06-06 12:49:11', 0),
(767, 1, '2026-06-06 20:00:00', '2026-06-06 20:59:59', 'Lunch and Science Fiction	 ', 1, 'Home', '2026-06-06 12:49:38', '2026-06-06 12:49:38', 0),
(768, 1, '2026-06-07 11:15:00', '2026-06-07 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-06-07 11:51:11', '2026-06-07 11:51:11', 0),
(769, 1, '2026-06-07 11:00:00', '2026-06-07 11:14:59', 'Load dishwash, start dishwash ', 1, 'Home', '2026-06-07 11:52:20', '2026-06-07 11:52:20', 0),
(770, 1, '2026-06-07 11:30:00', '2026-06-07 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-06-07 11:53:05', '2026-06-07 11:53:05', 0),
(771, 1, '2026-06-07 11:45:00', '2026-06-07 11:59:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-06-07 11:53:45', '2026-06-07 11:53:45', 0),
(772, 1, '2026-06-07 12:00:00', '2026-06-07 12:14:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-06-07 11:54:25', '2026-06-07 11:54:25', 0),
(773, 1, '2026-06-07 12:15:00', '2026-06-07 15:59:59', 'Refactor model windows, move table view code into window', 0, 'Starbucks', '2026-06-07 11:55:48', '2026-06-07 11:55:48', 0),
(774, 1, '2026-06-07 16:00:00', '2026-06-07 16:59:59', 'Breakfast and Science Fiction', 1, '', '2026-06-07 11:56:37', '2026-06-07 11:56:37', 0),
(775, 1, '2026-06-07 20:00:00', '2026-06-07 20:59:59', 'Lunch and Science Fiction	 ', 1, 'Home', '2026-06-07 11:57:45', '2026-06-07 11:57:45', 0),
(776, 1, '2026-06-09 11:00:00', '2026-06-09 11:14:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-06-09 19:18:47', '2026-06-09 19:18:47', 0),
(777, 1, '2026-06-09 11:15:00', '2026-06-09 11:29:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-06-09 19:19:16', '2026-06-09 19:19:16', 0),
(778, 1, '2026-06-09 11:30:00', '2026-06-09 11:44:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-06-09 19:19:56', '2026-06-09 19:19:56', 0),
(779, 1, '2026-06-09 12:00:00', '2026-06-09 12:30:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-06-09 19:20:26', '2026-06-09 19:20:26', 0),
(780, 1, '2026-06-09 12:30:00', '2026-06-09 15:59:59', 'Fix Note Windox bugs', 0, 'Starbucks', '2026-06-09 19:21:41', '2026-06-09 19:21:41', 0),
(781, 1, '2026-06-11 11:15:00', '2026-06-11 11:29:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-06-11 14:13:54', '2026-06-11 14:13:54', 0),
(782, 1, '2026-06-11 11:45:00', '2026-06-11 11:59:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-06-11 14:14:35', '2026-06-11 14:14:35', 0),
(783, 1, '2026-06-11 12:00:00', '2026-06-11 12:30:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-06-11 14:15:12', '2026-06-11 14:15:12', 0),
(784, 1, '2026-06-11 12:30:00', '2026-06-11 15:59:59', 'Move all code from table viewers to subwindows, remove table viewers.', 0, 'Starbucks', '2026-06-11 14:16:22', '2026-06-11 14:16:22', 0),
(785, 1, '2026-06-11 14:16:55', '2026-06-11 15:16:54', 'Test of add event', 0, '', '2026-06-11 14:17:15', '2026-06-11 14:17:15', 0),
(786, 1, '2026-06-11 16:00:00', '2026-06-11 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-06-11 14:17:49', '2026-06-11 14:17:49', 0),
(787, 1, '2026-06-11 17:00:00', '2026-06-11 17:59:59', 'Grocery shopping at Redondo Beach Ralph\'s Market', 1, 'Redondo Beach Ralph\'s Market', '2026-06-11 14:18:14', '2026-06-11 14:18:14', 0),
(788, 1, '2026-06-11 20:00:00', '2026-06-11 20:59:59', 'Lunch and Science Fiction	 ', 1, 'Home', '2026-06-11 14:18:40', '2026-06-11 14:18:40', 0),
(789, 1, '2026-06-12 01:00:00', '2026-06-12 01:59:59', 'Start automatic rice cooker', 1, 'Home', '2026-06-11 14:19:07', '2026-06-11 14:19:07', 0),
(790, 1, '2026-06-12 02:00:00', '2026-06-12 02:59:59', 'Cook and eat dinner', 1, 'Home', '2026-06-11 14:19:35', '2026-06-11 14:19:35', 0),
(791, 1, '2026-06-13 23:00:00', '2026-06-13 23:59:59', 'Eitan\'s Master Degree Graduation', 1, 'Home', '2026-06-11 14:20:25', '2026-06-11 14:20:25', 0),
(792, 1, '2026-06-13 16:00:00', '2026-06-13 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-06-11 14:20:53', '2026-06-11 14:20:53', 0),
(793, 1, '2026-06-13 20:00:00', '2026-06-13 20:59:59', 'Lunch and  Science Fiction', 1, 'Home', '2026-06-11 14:21:17', '2026-06-11 14:21:17', 0),
(794, 1, '2026-06-14 01:00:00', '2026-06-14 01:59:59', 'Cook and eat dinner', 1, 'Home', '2026-06-11 14:21:38', '2026-06-11 14:21:38', 0),
(795, 1, '2026-06-12 10:45:00', '2026-06-12 10:59:59', 'Check vital signs, log data on iPhone and in spreadsheet', 1, 'Home', '2026-06-12 11:34:10', '2026-06-12 11:34:10', 0),
(796, 1, '2026-06-12 11:00:00', '2026-06-12 11:14:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-06-12 11:35:04', '2026-06-12 11:35:04', 0),
(797, 1, '2026-06-12 11:15:00', '2026-06-12 11:44:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-06-12 11:36:10', '2026-06-12 11:36:10', 0),
(798, 1, '2026-06-12 11:45:00', '2026-06-12 11:59:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-06-12 11:36:44', '2026-06-12 11:36:44', 0),
(799, 1, '2026-06-12 12:00:00', '2026-06-12 15:59:59', 'DRY code in user dashboard. Then finish goal editor.', 0, 'Starbucks', '2026-06-12 11:38:39', '2026-06-12 11:38:39', 0),
(800, 1, '2026-06-12 16:00:00', '2026-06-12 16:59:59', 'Breakfast and Science Fiction', 1, 'Home', '2026-06-12 11:40:00', '2026-06-12 11:40:00', 0),
(801, 1, '2026-06-12 20:00:00', '2026-06-12 20:59:59', 'Lunch and Science Fiction	 ', 1, 'Home', '2026-06-12 11:40:38', '2026-06-12 11:40:38', 0),
(802, 1, '2026-06-13 01:00:00', '2026-06-13 01:59:59', 'Start automatic rice cooker', 1, 'Home', '2026-06-12 11:41:03', '2026-06-12 11:41:03', 0),
(803, 1, '2026-06-13 02:00:00', '2026-06-13 02:59:59', 'Cook and eat dinner', 1, 'Home', '2026-06-12 11:41:56', '2026-06-12 11:41:56', 0),
(804, 1, '2026-06-12 17:00:00', '2026-06-12 17:59:59', 'Testing add an event with the new Per Day View implementation', 0, 'Starbucks', '2026-06-12 13:06:00', '2026-06-12 13:06:00', 0),
(805, 1, '2026-06-13 11:15:00', '2026-06-13 11:29:59', 'Load dishwash, start dishwash ', 1, 'Home', '2026-06-13 11:59:18', '2026-06-13 11:59:18', 0),
(806, 1, '2026-06-13 11:30:00', '2026-06-13 11:44:59', 'Coffee, say hello to everyone ', 1, 'Starbucks', '2026-06-13 12:00:08', '2026-06-13 12:00:08', 0),
(807, 1, '2026-06-13 11:45:00', '2026-06-13 12:15:59', 'Read email, clear notifications on LinkedIn, plan day ', 1, 'Starbucks', '2026-06-13 12:00:49', '2026-06-13 12:00:49', 0),
(808, 1, '2026-06-13 12:15:00', '2026-06-13 12:30:59', 'Code Review moderator oversight. ', 0, 'Starbucks', '2026-06-13 12:01:44', '2026-06-13 12:01:44', 0),
(809, 1, '2026-06-13 12:30:00', '2026-06-13 15:59:59', 'Research on converting MariaDB database to sqlite.', 0, 'Starbucks', '2026-06-13 12:02:40', '2026-06-13 13:19:44', 0),
(810, 1, '2026-06-13 17:00:00', '2026-06-13 17:59:59', 'Get gas, checkout CostCo meats', 1, 'Costco', '2026-06-13 12:03:54', '2026-06-13 12:03:54', 0),
(811, 1, '2026-06-13 18:00:00', '2026-06-13 18:59:59', 'Pay American Express bill', 1, 'Home', '2026-06-13 12:58:35', '2026-06-13 12:58:35', 0);


END$$

DELIMITER ;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`AddTestData`;

CREATE PROCEDURE `testPTSDB`.`AddTestData`()

BEGIN

   CALL AddOrganization('Organization Name', 'anyemailaddress@anyURL.org', '(800) 555-1212', NULL, NULL, '1 Nowhere Street', 'Big City', 'Any state', 'postal code', 'any country');

   CALL AddTestUsers();

   CALL AddTestTasks();

   CALL AddTestNotes();

   CALL AddTestSchedule();

END$$

DELIMITER ;

DELIMITER $$

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`TestUserStoredProcedures`;

CREATE PROCEDURE `testPTSDB`.`TestUserStoredProcedures`()

BEGIN

   CALL GetAllUsers();

   CALL GetUserByID(1);

   CALL GetUserByLoginName('UserOne');

   CALL GetUserByEmail('UserOne@readyUserOne.com');

   CALL GetUserByLoginAndPassword('UserOne', 'testPassword');

   CALL GetUserByFullName('Shinny', 'Eric', 'Y');

   CALL UpdateUserAllFields(3, 1, 'AlteredLast', 'AlteredFirst', 'D', 'AlteredEmail', 'AlteredUname', 'AlteredPW', 'AlteredPreffs', '2026-04-17 22:00:00');

END$$

DELIMITER ;

DELIMITER $$
USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`TestTaskStoredProcedures`;

CREATE PROCEDURE `testPTSDB`.`TestTaskStoredProcedures`()

BEGIN

   CALL GetTaskByTaskID(1);

   CALL GetTaskByDescriptionAndAssignedUser(1, 'A task description');

   CALL GetActiveTasksForAssignedUser(1);

   CALL GetUnstartedDueForStartForAssignedUser(1, '2026-05-28');

   CALL GetTasksCompletedByAssignedAfterDate(1,'2024-05-14');

   CALL GetTasksByAssignedIDandParentID(1, 1);

   CALL GetDefaultDashboardTaskList(1, '2026-03-22');

   CALL HideTask(4, 59);

   CALL UpdateTaskAllFields(50, 4, 5, 'Install a WordPress Archive Plugin', 2, 4, '2025-05-01', '2025-05-01', '2025-05-11', '2025-05-12', '2025-05-11', 2, 1.5, 4, 3, 0, 1, '49;', 0);
   
   SELECT TaskID FROM Tasks  WHERE AsignedTo = 1 AND RequiredDelivery < '2026-03-22' AND Completed IS NULL AND (Hidden IS NULL OR Hidden <> 1) ORDER BY SchedulePriorityGroup 
ASC, PriorityInGroup ASC;

END$$

DELIMITER ;

DELIMITER $$

USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`TestNoteStoredProcedures`;

CREATE PROCEDURE `testPTSDB`.`TestNoteStoredProcedures`()

BEGIN

   CALL UpdateNoteAllFields(1, 25, 'An index for Location in schedule item is now added.');

   CALL HideNote(1, 12);

   CALL AddUserNote(1, 'Test AddUserNote SQL implementation.');

   CALL GetNoteByID(12);

   CALL GetAllNotesForUser(1);

   CALL GetAllUndeletedNotesForUser(1);

   CALL GetNotesForUserSimlarToContent(1, 'This is a test ');

   CALL GetAllNotesForUserCreatedInDatgeRange(1, '2026-03-12', '2026-05-14');

   CALL GetAllNotesForUserEditedInDatgeRange(1, '2026-03-12', '2026-05-14');

   CALL GetDashboardNoteTable(1, '2026-05-08 07:00:00.000000', '2026-05-09 06:59:00.000000');

END$$

DELIMITER ;

DELIMITER $$

USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`TestScheduleItemsStoredProcedures`;

CREATE PROCEDURE `testPTSDB`.`TestScheduleItemsStoredProcedures`()

BEGIN

   CALL AddScheduleEvent(1, '2026-04-17 22:00:00', '2026-04-17 23:00:00', 'Test AddScheduleEvent', 0, 'Starbucks');

   CALL HideScheduleItem(1, 53);

   SELECT idUserScheduleItem FROM UserScheduleItem  WHERE UserID = 1 AND (Hidden IS NULL OR Hidden <> 1) AND StartDateTime >= '2026-03-08 08:00:00.000000' AND StartDateTime <= '20
26-03-09 07:59:59.000000' ORDER BY StartDateTime ASC;

   CALL UpdateScheduleItemAllFields(1, 238, '2026-03-15 01:00:00', '2026-03-15 02:29:59', 'Sorry, no pea soup today.', 1, 'Norm\'s');

   CALL GetUserDaySchedule(1, '2024-05-14 07:00:00.000000', '2024-05-15 06:59:59.000000');

   CALL FindUserScheduleItemsByContentAndDateRange(1, 'Title search', '2024-05-07', '2024-05-21');

   CALL GetScheduleItemById(1);

END$$

DELIMITER ;

DELIMITER $$

USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`TestUserGoalStoredProcedures`;

CREATE PROCEDURE `testPTSDB`.`TestUserGoalStoredProcedures`()

BEGIN

   CALL AddUserGoal(1, 'Test AddUserGoal SQL implementation.', 0, 0);

   CALL UpdateUserGoalAllFields(1, 1, 'Test AddUserGoal SQL implementation.', 2, 1);

   CALL GetGoalById(1);

   CALL GetAllGoalsForUser(1);

   CALL FindGoalByUserIdAndExactDescription(1, 'Test AddUserGoal SQL implementation.');

   CALL GetAllChildGoalsFromParent(1, 1);

   CALL FindGoalsByUserIdAndSimilarDescription(1, 'SQL implementation.');

END$$

DELIMITER ;

DELIMITER $$

USE `testPTSDB`;
DROP PROCEDURE IF EXISTS `testPTSDB`.`TestStoredProceduresAndFunctions`;

CREATE PROCEDURE `testPTSDB`.`TestStoredProceduresAndFunctions`()

BEGIN

   CALL TestUserStoredProcedures();

   CALL TestTaskStoredProcedures();

   CALL TestScheduleItemsStoredProcedures();

   CALL TestNoteStoredProcedures();

   CALL TestUserGoalStoredProcedures();

END$$

DELIMITER ;

   CALL AddTestData();

#   CALL TestStoredProceduresAndFunctions();

