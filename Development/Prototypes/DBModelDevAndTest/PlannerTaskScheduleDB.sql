SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;

DROP DATABASE IF EXISTS test_ptsdb;
CREATE DATABASE test_ptsdb;

-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.organization_profile (
    id_organization INT UNSIGNED NOT NULL AUTO_INCREMENT,
    organization_name VARCHAR(256) NOT NULL,
    email_address VARCHAR(256) NOT NULL,
    phone_number VARCHAR(32) NOT NULL,
    primary_contact_user INT UNSIGNED,    # This should  be a foriegn key from user_profile, but that table does not exist yet
    secondary_contact_user INT UNSIGNED,  # This should  be a foriegn key from user_profile, but that table does not exist yet
    address_line_1 VARCHAR(256),
    address_line_2 VARCHAR(256),
    city VARCHAR(128),
    postal_code VARCHAR(32),
    state_or_province VARCHAR(256),
    nation VARCHAR(64),
    created_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (id_organization),
    INDEX organization_name_idx (organization_name ASC),
    INDEX fk_orgn_prime_contact_idx (primary_contact_user ASC)
);


-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.user_profile (
    user_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    id_organization INT UNSIGNED,
    last_name VARCHAR(45) NOT NULL,
    first_name VARCHAR(45) NOT NULL,
    middle_initial VARCHAR(45),
    email_address VARCHAR(256),
    user_login VARCHAR(45) NOT NULL,
    hashed_password VARCHAR(45) NOT NULL,
    preferences MEDIUMTEXT NOT NULL,
    created_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    last_login DATETIME,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (user_id),
    INDEX full_name_idx (last_name, first_name, middle_initial),
    UNIQUE INDEX user_login_unique (user_login ASC),
    UNIQUE INDEX email_unique (email_address ASC),
    INDEX up_last_login (last_login DESC),
    CONSTRAINT fk_user_profile_org_id
        FOREIGN KEY (id_organization)
        REFERENCES organization_profile (id_organization)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    INDEX fk_user_orgid_idx (id_organization)
);

-- --------------------------------------------------------

CREATE OR REPLACE TABLE  test_ptsdb.user_goals (
    id_user_goals INT UNSIGNED NOT NULL AUTO_INCREMENT,
    user_id INT UNSIGNED NOT NULL,
    description VARCHAR(1024) NOT NULL,
    priority INT DEFAULT NULL,
    parent_goal INT UNSIGNED DEFAULT NULL,
    creation_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (id_user_goals, user_id),
    UNIQUE INDEX id_user_goals_unique (id_user_goals ASC),
    INDEX ug_description_idx (description ASC),
    INDEX ug_creation_timestamp_idx (creation_timestamp DESC),
    INDEX ug_last_modified_time_stamp_idx (last_modified_time_stamp DESC),
    CONSTRAINT fk_user_goals_user_id
        FOREIGN KEY (user_id)
        REFERENCES user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    INDEX fk_user_goals_user_id_idx (user_id)
);

-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.user_notes (
    id_user_notes INT UNSIGNED NOT NULL AUTO_INCREMENT,
    user_id INT UNSIGNED NOT NULL,
    note_creation TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    content VARCHAR(1024) NOT NULL,
    last_modifed TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (id_user_notes, user_id),
    INDEX note_creation (note_creation DESC),
    INDEX user_notes_last_modifed (last_modifed DESC),
    INDEX un_content_idx (content ASC),
    INDEX fk_user_notes_user_id_idx (user_id ASC),
    CONSTRAINT fk_user_notes_user_id
      FOREIGN KEY (user_id)
      REFERENCES test_ptsdb.user_profile (user_id)
      ON DELETE RESTRICT
      ON UPDATE RESTRICT
);
    
-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.tasks (
    task_id INT UNSIGNED NOT NULL AUTO_INCREMENT,
    created_by INT UNSIGNED NOT NULL,
    assigned_to INT UNSIGNED NOT NULL,
    description VARCHAR(256) NOT NULL,
    parent_task INT UNSIGNED DEFAULT NULL,
    task_status INT UNSIGNED DEFAULT NULL,
    creation_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    due_date DATE NOT NULL,
    planned_start DATE NOT NULL,
    actual_start DATE DEFAULT NULL,
    estimated_delivery DATE DEFAULT NULL,
    delivered DATE DEFAULT NULL,
    est_hours_effort DOUBLE NOT NULL,
    hours_effort DOUBLE NOT NULL,
    priority_category INT UNSIGNED NOT NULL,
    priority INT UNSIGNED NOT NULL,
    personal TINYINT DEFAULT 0,
    dependency_count INT UNSIGNED,
    dependencies MEDIUMTEXT,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (task_id, created_by),
    INDEX fk_tasks_created_by_idx (created_by ASC),
    INDEX fk_tasks_assigned_to_idx (assigned_to ASC),
    INDEX task_description_idx (description ASC),
    INDEX task_last_modified_time_stamp_idx (last_modified_time_stamp DESC),
    INDEX task_priority_category_idx (priority_category ASC),
    INDEX task_priority_idx (priority ASC),
    INDEX task_task_status_idx (task_status ASC),
    INDEX task_due_date_idx (due_date ASC),
    INDEX task_planned_start_idx (planned_start ASC),
    CONSTRAINT fk_tasks_created_by
        FOREIGN KEY (created_by)
        REFERENCES user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT fk_tasks_assigned_to
        FOREIGN KEY (assigned_to)
        REFERENCES user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);


-- --------------------------------------------------------

CREATE OR REPLACE TABLE  test_ptsdb.user_task_goals (
    user_id INT UNSIGNED NOT NULL,
    task_id  INT UNSIGNED NOT NULL,
    task_goal_list VARCHAR(45) NOT NULL,
    creation_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (user_id, task_id),
    INDEX utg_task_idx (task_id ASC),
    INDEX utg_creation_timestamp_idx (creation_timestamp DESC),
    INDEX utg_last_modified_time_stamp_idx (last_modified_time_stamp DESC),
    CONSTRAINT fk_user_task_goals_assigned_to
        FOREIGN KEY (user_id)
        REFERENCES user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT,
    CONSTRAINT fk_user_task_goals_task_id
        FOREIGN KEY (task_id)
        REFERENCES tasks (task_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);


-- --------------------------------------------------------

CREATE OR REPLACE TABLE test_ptsdb.user_schedule_item (
    id_user_schedule_item INT UNSIGNED NOT NULL AUTO_INCREMENT,
    user_id INT UNSIGNED NOT NULL,
    start_date_time DATETIME NOT NULL,
    end_date_time DATETIME NOT NULL,
    title VARCHAR(128) NOT NULL,
    personal TINYINT NOT NULL,
    location VARCHAR(128) DEFAULT NULL,
    created_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    last_modified_time_stamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    deleted TINYINT DEFAULT 0,    # Records are never deleted but they can be hidden.
    PRIMARY KEY (id_user_schedule_item, user_id),
    INDEX schedule_item_title_idx (title ASC),
    INDEX schedule_item_start_date_time_idx (start_date_time DESC),
    INDEX schedule_item_created_timestamp_idx (created_timestamp DESC),
    INDEX schedule_item_last_modified_time_stamp_idx (last_modified_time_stamp DESC),
    INDEX fk_user_schedule_item_user_id_idx (user_id ASC),
    INDEX schedule_item_location_idx (location ASC),
    CONSTRAINT fk_user_schedule_item_user_id
        FOREIGN KEY (user_id)
        REFERENCES test_ptsdb.user_profile (user_id)
        ON DELETE RESTRICT
        ON UPDATE RESTRICT
);

COMMIT;


