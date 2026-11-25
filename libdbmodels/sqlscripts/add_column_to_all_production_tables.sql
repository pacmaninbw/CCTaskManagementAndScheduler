START TRANSACTION;

ALTER TABLE production_ptsdb.organization_profile 
ADD COLUMN last_modified_by_user INT UNSIGNED NOT NULL DEFAULT 1,
ADD CONSTRAINT fk_organization_last_modified_by_user 
    FOREIGN KEY (last_modified_by_user) 
    REFERENCES production_ptsdb.user_profile (user_id)
    ON DELETE RESTRICT
    ON UPDATE CASCADE;

ALTER TABLE production_ptsdb.user_profile 
ADD COLUMN last_modified_by_user INT UNSIGNED NOT NULL DEFAULT 1,
ADD CONSTRAINT fk_user_profile_last_modified_by_user 
    FOREIGN KEY (last_modified_by_user) 
    REFERENCES production_ptsdb.user_profile (user_id)
    ON DELETE RESTRICT
    ON UPDATE CASCADE;

ALTER TABLE production_ptsdb.user_goals 
ADD COLUMN last_modified_by_user INT UNSIGNED NOT NULL DEFAULT 1,
ADD CONSTRAINT fk_user_goals_last_modified_by_user 
    FOREIGN KEY (last_modified_by_user) 
    REFERENCES production_ptsdb.user_profile (user_id)
    ON DELETE RESTRICT
    ON UPDATE CASCADE;

ALTER TABLE production_ptsdb.user_notes 
ADD COLUMN last_modified_by_user INT UNSIGNED NOT NULL DEFAULT 1,
ADD CONSTRAINT fk_user_notes_last_modified_by_user 
    FOREIGN KEY (last_modified_by_user) 
    REFERENCES production_ptsdb.user_profile (user_id)
    ON DELETE RESTRICT
    ON UPDATE CASCADE;

ALTER TABLE production_ptsdb.tasks 
ADD COLUMN last_modified_by_user INT UNSIGNED NOT NULL DEFAULT 1,
ADD CONSTRAINT fk_tasks_last_modified_by_user 
    FOREIGN KEY (last_modified_by_user) 
    REFERENCES production_ptsdb.user_profile (user_id)
    ON DELETE RESTRICT
    ON UPDATE CASCADE;

ALTER TABLE production_ptsdb.user_task_goals 
ADD COLUMN last_modified_by_user INT UNSIGNED NOT NULL DEFAULT 1,
ADD CONSTRAINT fk_user_task_goals_last_modified_by_user 
    FOREIGN KEY (last_modified_by_user) 
    REFERENCES production_ptsdb.user_profile (user_id)
    ON DELETE RESTRICT
    ON UPDATE CASCADE;

ALTER TABLE production_ptsdb.user_schedule_item 
ADD COLUMN last_modified_by_user INT UNSIGNED NOT NULL DEFAULT 1,
ADD CONSTRAINT fk_user_schedule_item_last_modified_by_user 
    FOREIGN KEY (last_modified_by_user) 
    REFERENCES production_ptsdb.user_profile (user_id)
    ON DELETE RESTRICT
    ON UPDATE CASCADE;

COMMIT;

